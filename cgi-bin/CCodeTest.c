#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // **Required for CGI**
    printf("Content-Type: application/json\r\n\r\n");

    char *query = getenv("QUERY_STRING"); // Get query string from URL
    if (!query) {
        printf("{\"status\":\"error\",\"message\":\"No data received\"}\n");
        return 0;
    }

    // Default values
    int calories = 0;
    const int GOAL = 2000;

    // Parse query string: calories=123
    if (sscanf(query, "calories=%d", &calories) != 1) {
        printf("{\"status\":\"error\",\"message\":\"Invalid input\"}\n");
        return 0;
    }

    // Calculate remaining
    int remaining = GOAL - calories;

    // Build JSON result
    if (remaining > 0) {
        printf("{\"status\":\"ok\",\"result\":\"%d calories remaining out of %d\"}\n", remaining, GOAL);
    } else if (remaining == 0) {
        printf("{\"status\":\"ok\",\"result\":\"You've hit the %d calorie goal exactly\"}\n", GOAL);
    } else {
        printf("{\"status\":\"ok\",\"result\":\"You are %d calories over the %d calorie goal\"}\n", -remaining, GOAL);
    }

    return 0;
}
