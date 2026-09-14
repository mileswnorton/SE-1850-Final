#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

double feetInchToCm(int feet, int inches);
double calorieNeeds(double weightKg, double heightCm, int age, char sex);
double lbsToKG(double lbs);
double activityMultiplier(const char *activity);

// Helper to get value from query string
int get_query_value(const char *query, const char *key, char *out, int maxLen) {
    char *found = strstr(query, key);
    if (!found) return 0;

    found += strlen(key); // skip key
    if (*found != '=') return 0; // ensure '='
    found++; // skip '='

    int i = 0;
    while (*found && *found != '&' && i < maxLen - 1) {
        out[i++] = *found++;
    }
    out[i] = '\0';
    return 1;
}

int main(void) {
    printf("Content-Type: application/json\r\n\r\n");

    // Read QUERY_STRING from environment
    char *query = getenv("QUERY_STRING");
    if (!query) {
        printf("{\"status\":\"error\",\"message\":\"No query string\"}\n");
        return 1;
    }

    char buf[64];

    // Parse inputs
    double weight = 0;
    int heightFeet = 0, heightInches = 0, age = 0, deficitModifier = 0;
    char sex = 'M';
    char activity[32] = "sedentary";

    if (get_query_value(query, "weight", buf, sizeof(buf))) weight = atof(buf);
    if (get_query_value(query, "heightFeet", buf, sizeof(buf))) heightFeet = atoi(buf);
    if (get_query_value(query, "heightInches", buf, sizeof(buf))) heightInches = atoi(buf);
    if (get_query_value(query, "age", buf, sizeof(buf))) age = atoi(buf);
    if (get_query_value(query, "deficitModifier", buf, sizeof(buf))) deficitModifier = atoi(buf);
    if (get_query_value(query, "sex", buf, sizeof(buf))) sex = toupper(buf[0]);
    if (get_query_value(query, "activity", buf, sizeof(buf))) strncpy(activity, buf, sizeof(activity)-1);

    // Calculate BMR
    double bmr = calorieNeeds(lbsToKG(weight), feetInchToCm(heightFeet, heightInches), age, sex);
    bmr *= activityMultiplier(activity);
    bmr += deficitModifier * -500;

    // Example protein calculation (e.g., 1 g protein per lb)
    double dailyProtein = weight;

    printf("{\"status\":\"ok\",\"DailyCalories\":%.2lf,\"DailyProtein\":%.2lf}\n", bmr, dailyProtein);

    return 0;
}

double calorieNeeds(double weightKg, double heightCm, int age, char sex) {
    double bmr = 10.0 * weightKg + 6.25 * heightCm - 5.0 * age;
    if (sex == 'M') bmr += 5.0;
    else if (sex == 'F') bmr -= 161.0;
    return bmr;
}

double lbsToKG(double lbs) {
    return lbs * 0.45359237;
}

double feetInchToCm(int feet, int inches) {
    return (feet * 12 + inches) * 2.54;
}

double activityMultiplier(const char *activity) {
    if (strcmp(activity, "sedentary") == 0) return 1.2;
    if (strcmp(activity, "light") == 0) return 1.375;
    if (strcmp(activity, "moderate") == 0) return 1.55;
    if (strcmp(activity, "active") == 0) return 1.725;
    if (strcmp(activity, "very_active") == 0) return 1.9;
    return 1.0;
}
