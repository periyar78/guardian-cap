// ══════════════════════════════════════════════════════════════
//  dashboard.js  —  UI update logic for SafeHelm dashboard
// ══════════════════════════════════════════════════════════════

// ─── MAP SETUP ────────────────────────────────────────────────
const map = L.map('map', { zoomControl: false, attributionControl: false })
              .setView([13.0827, 80.2707], 17);

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
  maxZoom: 19
}).addTo(map);

L.control.zoom({ position: 'bottomright' }).addTo(map);

const workerIcon = L.divIcon({
  className: '',
  html: `<div style="
    width:14px;height:14px;
    background:#f97316;border-radius:50%;
    border:3px solid #fff;
    box-shadow:0 0 0 3px rgba(249,115,22,0.4);
  "></div>`,
  iconSize: [14, 14],
  iconAnchor: [7, 7]
});

let marker = null;
let path   = [];
let polyline = null;

function updateMap(lat, lng, heading) {
  if (!marker) {
    marker = L.marker([lat, lng], { icon: workerIcon }).addTo(map);
  } else {
    marker.setLatLng([lat, lng]);
  }
  map.panTo([lat, lng], { animate: true, duration: 0.8 });

  // Trail
  path.push([lat, lng]);
  if (path.length > 120) path.shift();
  if (polyline) map.removeLayer(polyline);
  polyline = L.polyline(path, {
    color: 'rgba(249,115,22,0.6)',
    weight: 2,
    dashArray: '4 4'
  }).addTo(map);
}

// ─── GAS CHART ────────────────────────────────────────────────
const GAS_HISTORY_LEN = 60;
const gasHistory = Array(GAS_HISTORY_LEN).fill(0);
const gasLabels  = Array(GAS_HISTORY_LEN).fill('');

const gasCtx = document.getElementById('gasChart').getContext('2d');
const gasChart = new Chart(gasCtx, {
  type: 'line',
  data: {
    labels: gasLabels,
    datasets: [{
      label: 'Gas ADC',
      data: gasHistory,
      borderColor: '#f97316',
      backgroundColor: 'rgba(249,115,22,0.08)',
      borderWidth: 1.5,
      pointRadius: 0,
      fill: true,
      tension: 0.4
    }]
  },
  options: {
    responsive: true,
    maintainAspectRatio: false,
    animation: { duration: 300 },
    plugins: {
      legend: { display: false },
      tooltip: {
        callbacks: {
          label: ctx => `Gas: ${ctx.raw} ADC`
        },
        backgroundColor: '#111318',
        borderColor: '#242830',
        borderWidth: 1,
        titleColor: '#6b7280',
        bodyColor: '#e8eaf0'
      }
    },
    scales: {
      x: {
        ticks: { display: false },
        grid: { color: 'rgba(255,255,255,0.04)' },
        border: { display: false }
      },
      y: {
        min: 0,
        max: 4095,
        ticks: {
          color: '#6b7280',
          font: { size: 10, family: "'Share Tech Mono'" },
          stepSize: 1000
        },
        grid: {
          color: 'rgba(255,255,255,0.04)',
          drawBorder: false
        },
        border: { display: false }
      }
    }
  }
});

// ─── DANGER ZONE annotation lines
function addThresholdLines() {
  // Using Chart.js plugin-annotation would be ideal; we draw them via canvas instead
  const plugin = {
    id: 'thresholdLines',
    afterDraw(chart) {
      const { ctx, chartArea, scales } = chart;
      if (!chartArea) return;
      const drawLine = (val, color) => {
        const y = scales.y.getPixelForValue(val);
        ctx.save();
        ctx.strokeStyle = color;
        ctx.lineWidth   = 1;
        ctx.setLineDash([4, 4]);
        ctx.globalAlpha = 0.5;
        ctx.beginPath();
        ctx.moveTo(chartArea.left, y);
        ctx.lineTo(chartArea.right, y);
        ctx.stroke();
        ctx.restore();
      };
      drawLine(1200,  '#eab308');
      drawLine(2000, '#ef4444');
    }
  };
  Chart.register(plugin);
}
addThresholdLines();

// ─── HELPERS ──────────────────────────────────────────────────
function setCardState(cardId, state) {
  const card = document.getElementById(cardId);
  card.classList.remove('safe', 'warn', 'danger');
  card.classList.add(state);
}

function setStatus(el, state, text) {
  el.className = `sensor-status ${state}`;
  el.innerHTML = `<div class="status-dot ${state}"></div>${text}`;
}

function now() {
  return new Date().toLocaleTimeString('en-IN', { hour12: false });
}

// ─── EVENT LOG ────────────────────────────────────────────────
let eventCount = 0;
const MAX_LOG  = 50;

function logEvent(type, desc) {
  eventCount++;
  const list = document.getElementById('events-list');
  const row  = document.createElement('div');
  row.className = 'event-row';
  row.innerHTML = `
    <span class="event-time">${now()}</span>
    <span class="event-type ${type}">${type.toUpperCase()}</span>
    <span class="event-desc">${desc}</span>
  `;
  list.prepend(row);
  // Trim old entries
  while (list.children.length > MAX_LOG) list.removeChild(list.lastChild);
  document.getElementById('event-count').textContent = `${eventCount} events`;
}

// ─── ALERT BANNER ─────────────────────────────────────────────
function updateAlertBanner(alerts) {
  const banner = document.getElementById('alert-banner');
  const items  = document.getElementById('alert-items');
  if (alerts.length === 0) {
    banner.classList.remove('active');
    return;
  }
  banner.classList.add('active');
  items.innerHTML = alerts.map(a => `<span class="alert-pill">${a}</span>`).join('');
}

// ─── UPTIME ───────────────────────────────────────────────────
const startTime = Date.now();
function formatUptime() {
  const s = Math.floor((Date.now() - startTime) / 1000);
  const h = String(Math.floor(s / 3600)).padStart(2, '0');
  const m = String(Math.floor((s % 3600) / 60)).padStart(2, '0');
  const sec = String(s % 60).padStart(2, '0');
  return `${h}:${m}:${sec}`;
}

// ─── CLOCK ────────────────────────────────────────────────────
setInterval(() => {
  document.getElementById('clock').textContent =
    new Date().toLocaleTimeString('en-IN', { hour12: false });
  document.getElementById('uptime').textContent = formatUptime();
}, 1000);

// ─── PREVIOUS STATE (for change detection) ───────────────────
let _prev = { gasState: null, helmetOff: null, fallDetected: null };

// ─── MAIN DATA HANDLER ────────────────────────────────────────
function onData(d) {
  // ── Gas ──────────────────────────────────────────────────────
  const gasRaw = d.gas;
  let gasState  = 'safe';
  let gasTxt    = 'Safe';
  if (gasRaw > 2000) { gasState = 'danger'; gasTxt = 'DANGER'; }
  else if (gasRaw > 1200) { gasState = 'warn';   gasTxt = 'Warning'; }

  document.getElementById('gas-value').innerHTML = `${gasRaw}<small>ADC</small>`;
  setCardState('card-gas', gasState);
  setStatus(document.getElementById('gas-status'), gasState, gasTxt);

  const pct = Math.min(100, (gasRaw / 4095) * 100);
  const bar  = document.getElementById('gas-bar');
  bar.style.width = pct + '%';
  bar.style.background = gasState === 'danger' ? '#ef4444' : gasState === 'warn' ? '#eab308' : '#22c55e';

  // Gas history chart
  gasHistory.push(gasRaw);
  if (gasHistory.length > GAS_HISTORY_LEN) gasHistory.shift();
  gasChart.data.datasets[0].data = [...gasHistory];
  // Color gradient based on level
  gasChart.data.datasets[0].borderColor =
    gasState === 'danger' ? '#ef4444' : gasState === 'warn' ? '#eab308' : '#f97316';
  gasChart.update('none');

  // ── Helmet ───────────────────────────────────────────────────
  const helmetOff = d.helmetOff;
  const helmetState = helmetOff ? 'danger' : 'safe';
  document.getElementById('helmet-value').textContent = helmetOff ? 'NOT WORN' : 'WORN';
  setCardState('card-helmet', helmetState);
  setStatus(document.getElementById('helmet-status'), helmetState,
    helmetOff ? 'Helmet removed!' : 'Worn correctly');

  // ── Fall ─────────────────────────────────────────────────────
  const fallState = d.fallDetected ? 'danger' : 'safe';
  document.getElementById('fall-accel').innerHTML =
    `${d.totalAccel.toFixed(2)}<small>m/s²</small>`;
  document.getElementById('fall-axes').textContent =
    `X: ${d.accelX.toFixed(2)}  Y: ${d.accelY.toFixed(2)}  Z: ${d.accelZ.toFixed(2)}`;
  setCardState('card-fall', fallState);
  setStatus(document.getElementById('fall-status'), fallState,
    d.fallDetected ? '⚠ FALL DETECTED' : 'Normal');

  // ── Vibration Motor ──────────────────────────────────────────
  const motorState = d.motorOn ? 'danger' : 'safe';
  document.getElementById('vib-value').textContent = d.motorOn ? 'ON' : 'OFF';
  setCardState('card-vib', motorState);
  setStatus(document.getElementById('vib-status'), motorState,
    d.motorOn ? 'Alerting worker' : 'No alert');

  // ── GPS ──────────────────────────────────────────────────────
  if (d.lat && d.lng) {
    updateMap(d.lat, d.lng, d.heading);
    document.getElementById('gps-text').textContent =
      `${d.lat.toFixed(5)}, ${d.lng.toFixed(5)}`;
    document.getElementById('gps-alt').textContent    = d.altitude.toFixed(1);
    document.getElementById('gps-heading').textContent = d.heading.toFixed(0) + '°';
    document.getElementById('gps-sat').textContent     = d.satellites;
    document.getElementById('satellites').textContent  = d.satellites + ' sat';
  }

  // ── Sidebar ping ─────────────────────────────────────────────
  document.getElementById('last-ping').textContent = now();

  // ── Alert banner ─────────────────────────────────────────────
  const alerts = [];
  if (gasState === 'danger')  alerts.push('Gas Danger');
  if (gasState === 'warn')    alerts.push('Gas Warning');
  if (d.fallDetected)         alerts.push('Fall Detected');
  if (helmetOff)              alerts.push('Helmet Off');
  updateAlertBanner(alerts);

  // ── Event logging (on state changes only) ────────────────────
  if (_prev.gasState !== gasState) {
    if (gasState === 'danger') logEvent('danger', `Gas level CRITICAL: ${gasRaw} ADC (>1200 threshold)`);
    else if (gasState === 'warn') logEvent('warn', `Gas level elevated: ${gasRaw} ADC (600–1200 range)`);
    else if (_prev.gasState !== null) logEvent('safe', `Gas level returned to normal: ${gasRaw} ADC`);
  }
  if (_prev.helmetOff !== helmetOff) {
    if (helmetOff) logEvent('danger', 'Helmet removed — worker unprotected!');
    else           logEvent('safe',   'Helmet put back on. Worker protected.');
  }
  if (_prev.fallDetected !== d.fallDetected) {
    if (d.fallDetected) logEvent('danger', `Fall detected! Total accel: ${d.totalAccel.toFixed(2)} m/s²`);
    else                logEvent('info',   'Fall condition cleared. Worker stable.');
  }

  _prev = { gasState, helmetOff, fallDetected: d.fallDetected };
}

// ─── SUBSCRIBE ────────────────────────────────────────────────
subscribeToHelmetData(onData);
logEvent('info', 'Dashboard loaded. Connecting to ESP32 data stream...');
