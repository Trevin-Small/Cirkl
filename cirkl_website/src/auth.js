import { getAuth, signInWithEmailAndPassword } from 'firebase/auth';

const auth = getAuth();

export async function signIn() {

  const email = document.getElementById('email').value;
  const password = document.getElementById('pass').value;

  await signInWithEmailAndPassword(auth, email, password).catch((err) => {

    console.log(err);

  });

}