function roll(elementId) {
    return document.getElementById(elementId).textContent = Math.floor(Math.random() * 20 + 1);
}
