// firebase-init.js
import { initializeApp } from "https://www.gstatic.com/firebasejs/12.6.0/firebase-app.js";
import { getAnalytics } from "https://www.gstatic.com/firebasejs/12.6.0/firebase-analytics.js";

import {
  getAuth,
  createUserWithEmailAndPassword,
  signInWithEmailAndPassword,
  signOut,
  onAuthStateChanged
} from "https://www.gstatic.com/firebasejs/12.6.0/firebase-auth.js";

import {
  getFirestore,
  doc,
  setDoc,
  getDoc,
  collection,
  addDoc,
  query,
  orderBy,
  getDocs,
  serverTimestamp
} from "https://www.gstatic.com/firebasejs/12.6.0/firebase-firestore.js";

// ⭐ ADD THIS IMPORT FOR STORAGE ⭐
import {
  getStorage
} from "https://www.gstatic.com/firebasejs/12.6.0/firebase-storage.js";


// ---- your firebaseConfig ----
const firebaseConfig = {
  apiKey: "AIzaSyB3dHaP7fbGYU-LxQR8ICefFax2SUysXvg",
  authDomain: "calorie-tracker-final.firebaseapp.com",
  projectId: "calorie-tracker-final",
  storageBucket: "calorie-tracker-final.appspot.com",
  messagingSenderId: "1057961241936",
  appId: "1:1057961241936:web:9da227d5f46796cd9b1055",
  measurementId: "G-Z2VHG69BHW"
};

// init
const app = initializeApp(firebaseConfig);
getAnalytics(app);

export const auth = getAuth(app);
export const db = getFirestore(app);

// ⭐ ADD THIS — STORAGE OBJECT ⭐
export const storage = getStorage(app);


/* -----------------------
   AUTH / USER helpers
------------------------*/

export async function signUp(name, email, password) {
  const userCred = await createUserWithEmailAndPassword(auth, email, password);
  const user = userCred.user;

  await setDoc(doc(db, "users", user.uid), {
    name,
    email,
    createdAt: serverTimestamp()
  });

  return user;
}

export async function logIn(email, password) {
  const userCred = await signInWithEmailAndPassword(auth, email, password);
  return userCred.user;
}

export async function logOut() {
  await signOut(auth);
}

export function onAuthChange(cb) {
  return onAuthStateChanged(auth, cb);
}

/* -----------------------
   FOOD LOG helpers
------------------------*/

export async function addFoodEntry(uid, entry) {
  const colRef = collection(db, "users", uid, "foodLogs");
  const docRef = await addDoc(colRef, {
    ...entry,
    createdAt: serverTimestamp()
  });
  return docRef.id;
}

export async function fetchFoodEntries(uid) {
  const colRef = collection(db, "users", uid, "foodLogs");
  const q = query(colRef, orderBy("createdAt", "desc"));
  const snap = await getDocs(q);
  return snap.docs.map(d => ({ id: d.id, ...d.data() }));
}
