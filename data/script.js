// === DOM ELEMENTS ===
const listBtn = document.getElementById("listBtn");
const pathInput = document.getElementById("pathInput");
const fileList = document.getElementById("fileList");
const fileViewer = document.getElementById("fileViewer");
const fileName = document.getElementById("fileName");
const fileContent = document.getElementById("fileContent");
const closeViewer = document.getElementById("closeViewer");
const ipModal = document.getElementById("ipModal");
const ipSettingsBtn = document.getElementById("ipSettingsBtn");
const ipInput = document.getElementById("ipInput");
const saveIpBtn = document.getElementById("saveIpBtn");
const closeIpBtn = document.getElementById("closeIpBtn");
const ipDisplay = document.getElementById("ipDisplay");

// === LOCAL STORAGE ===
function getBaseURL() {
  const savedIp = localStorage.getItem("esp32_ip") || "";
  return savedIp ? `http://${savedIp}` : "";
}

function updateIpDisplay() {
  const ip = localStorage.getItem("esp32_ip");
  const span = ipDisplay.querySelector("span");
  if (ip) {
    span.textContent = `http://${ip}`;
    span.classList.add("text-blue-400");
  } else {
    span.textContent = "No IP configured";
    span.classList.remove("text-blue-400");
  }
}

// === MODAL HANDLERS ===
ipSettingsBtn.addEventListener("click", () => {
  ipModal.classList.remove("hidden");
  const saved = localStorage.getItem("esp32_ip") || "";
  ipInput.value = saved;
  updateIpDisplay();
});

closeIpBtn.addEventListener("click", () => {
  ipModal.classList.add("hidden");
});

saveIpBtn.addEventListener("click", () => {
  const ip = ipInput.value.trim();
  if (!ip) return alert("Please enter a valid IP address!");
  localStorage.setItem("esp32_ip", ip);
  updateIpDisplay();
  ipModal.classList.add("hidden");
});

window.addEventListener("DOMContentLoaded", updateIpDisplay);

// === FILE BROWSER ===
async function fetchListDir() {
  const base = getBaseURL();
  if (!base) {
    alert("Please configure IP first!");
    return;
  }

  const path = pathInput.value.trim() || "/";
  fileList.innerHTML = `<p class="text-center text-gray-400 text-sm py-3 animate-pulse">Loading...</p>`;

  try {
    const res = await fetch(`${base}/api/listdir?path=${encodeURIComponent(path)}`);
    const text = await res.text();
    const lines = text.trim().split("\n").filter(Boolean);

    fileList.innerHTML = "";
    if (lines.length === 0) {
      fileList.innerHTML = `<p class="text-center text-gray-500 text-sm py-3">Empty directory</p>`;
      return;
    }

    const ul = document.createElement("ul");
    ul.className = "divide-y divide-gray-800";

    lines.forEach((line) => {
      const isDir = line.endsWith("/");
      const li = document.createElement("li");
      li.className =
        "flex items-center justify-between px-3 py-2 hover:bg-gray-800 rounded-md cursor-pointer transition";

      const left = document.createElement("div");
      left.className = "flex items-center gap-2";

      const icon = document.createElement("span");
      icon.textContent = isDir ? "📁" : "📄";

      const name = document.createElement("span");
      name.textContent = line.replace("/", "");
      name.className = "truncate";

      left.appendChild(icon);
      left.appendChild(name);
      li.appendChild(left);

      // Action on click
      li.addEventListener("click", async () => {
        if (isDir) {
          pathInput.value =
            (path.endsWith("/") ? path : path + "/") + line.replace("/", "");
          await fetchListDir();
        } else {
          await openFileViewer((path.endsWith("/") ? path : path + "/") + line);
        }
      });

      ul.appendChild(li);
    });

    fileList.appendChild(ul);
  } catch (err) {
    fileList.innerHTML = `<p class="text-red-500 text-sm text-center py-3">Error: ${err.message}</p>`;
  }
}

// === FILE VIEWER ===
async function openFileViewer(fullPath) {
  const base = getBaseURL();
  fileViewer.classList.remove("hidden");
  fileName.textContent = fullPath;
  fileContent.textContent = "Loading...";

  try {
    const res = await fetch(`${base}/api/readfile?path=${encodeURIComponent(fullPath)}`);
    const text = await res.text();
    fileContent.textContent = text || "[Empty File]";
  } catch (err) {
    fileContent.textContent = `Error: ${err.message}`;
  }
}

closeViewer.addEventListener("click", () => {
  fileViewer.classList.add("hidden");
  fileContent.textContent = "";
  fileName.textContent = "";
});

// === EVENT ===
listBtn.addEventListener("click", fetchListDir);
