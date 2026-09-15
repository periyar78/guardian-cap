// ══════════════════════════════════════════════════════════════
//  firebase.js  —  Firebase Realtime Database integration
//  Replace the config below with your own Firebase project values
// ══════════════════════════════════════════════════════════════

// ─── YOUR FIREBASE CONFIG ────────────────────────────────────
const FIREBASE_CONFIG = {
  apiKey:            "YOUR_API_KEY",
  authDomain:        "YOUR_PROJECT.firebaseapp.com",
  databaseURL:       "https://YOUR_PROJECT-default-rtdb.firebaseio.com",
  projectId:         "YOUR_PROJECT",
  storageBucket:     "YOUR_PROJECT.appspot.com",
  messagingSenderId: "YOUR_SENDER_ID",
  appId:             "YOUR_APP_ID"
};

// ─── PATH in the Realtime DB where ESP32 writes ──────────────
const DB_PATH = "helmet/worker_1/latest";

// ─── Firebase SDK (loaded via CDN in index.html) ─────────────
// To use real Firebase, add these two script tags to index.html BEFORE firebase.js:
//
//   <script src="https://www.gstatic.com/firebasejs/10.12.0/firebase-app-compat.js"></script>
//   <script src="https://www.gstatic.com/firebasejs/10.12.0/firebase-database-compat.js"></script>
//
// Then uncomment the block below:

/*
firebase.initializeApp(FIREBASE_CONFIG);
const db = firebase.database();

function subscribeToHelmetData(callback) {
  const ref = db.ref(DB_PATH);
  ref.on('value', snapshot => {
    const data = snapshot.val();
    if (data) callback(data);
  });
}
*/

// ─── DEMO / SIMULATION MODE ───────────────────────────────────
// When Firebase is not yet configured this simulator generates
// realistic sensor readings so the dashboard is fully functional.

let _simStartTime  = Date.now();
let _simGasBase    = 400;
let _simFallPhase  = 0;
let _simHelmet     = true;

function _simulateData() {
  const t = (Date.now() - _simStartTime) / 1000;

  // Slow gas drift with occasional spike
  _simGasBase += (Math.random() - 0.48) * 40;
  _simGasBase  = Math.max(100, Math.min(3800, _simGasBase));
  // Random spike every ~30 s
  const spike  = (Math.floor(t / 30) % 2 === 1) ? 800 : 0;
  const gas    = Math.round(_simGasBase + spike + (Math.random() - 0.5) * 60);

  // Helmet toggled every 50 s
  if (Math.floor(t / 50) % 3 === 2) _simHelmet = false;
  else _simHelmet = true;

  // Sinusoidal accelerometer baseline + rare fall event
  _simFallPhase += 0.08;
  const isFall  = (Math.floor(t / 70) % 4 === 3);
  const az      = isFall ? 1.2 : 9.61 + Math.sin(_simFallPhase) * 0.3;
  const ax      = (Math.random() - 0.5) * (isFall ? 8 : 0.5);
  const ay      = (Math.random() - 0.5) * (isFall ? 6 : 0.5);
  const totalAccel = Math.sqrt(ax*ax + ay*ay + az*az);

  // GPS: fixed to Chennai refineries area + micro drift
  const lat = 13.0827 + (Math.random() - 0.5) * 0.0003;
  const lng = 80.2707 + (Math.random() - 0.5) * 0.0003;

  return {
    gas:        gas,
    helmetOff:  !_simHelmet,
    accelX:     parseFloat(ax.toFixed(3)),
    accelY:     parseFloat(ay.toFixed(3)),
    accelZ:     parseFloat(az.toFixed(3)),
    totalAccel: parseFloat(totalAccel.toFixed(3)),
    fallDetected: isFall || totalAccel < 2.0 || totalAccel > 15.0,
    motorOn:    (gas > 1200 || isFall),
    lat:        parseFloat(lat.toFixed(6)),
    lng:        parseFloat(lng.toFixed(6)),
    altitude:   parseFloat((8.5 + Math.random() * 0.5).toFixed(1)),
    heading:    parseFloat((Math.random() * 360).toFixed(1)),
    satellites: Math.floor(6 + Math.random() * 6),
    timestamp:  Date.now()
  };
}


