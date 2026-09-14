import { signUp, logIn } from "./firebase-init.js";

// SIGN UP HANDLER
async function handleSignup() {
  const name = document.getElementById("signup-name").value;
  const email = document.getElementById("signup-email").value;
  const password = document.getElementById("signup-password").value;

  try {
    await signUp(name, email, password);
    alert("Account created!");
    window.location.href = "index.html"; // redirect after signup
  } catch (err) {
    alert("Error: " + err.message);
  }
}

// LOGIN HANDLER
async function handleLogin() {
  const email = document.getElementById("login-email").value;
  const password = document.getElementById("login-password").value;

  try {
    await logIn(email, password);
    alert("Logged in!");
    window.location.href = "index.html";
  } catch (err) {
    alert("Error: " + err.message);
  }
}

// expose functions to HTML
window.handleSignup = handleSignup;
window.handleLogin = handleLogin;
