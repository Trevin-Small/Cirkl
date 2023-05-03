import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
import { Main } from './main.js';
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyDvn53F6_u2YxB5KQJXRbn3XJ-EwfjoVUo",
  authDomain: "cirkl-d3b3d.firebaseapp.com",
  projectId: "cirkl-d3b3d",
  storageBucket: "cirkl-d3b3d.appspot.com",
  messagingSenderId: "791478699120",
  appId: "1:791478699120:web:852c432fa603d42f9060b5",
  measurementId: "G-5S303FP0SY"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

Main.init();