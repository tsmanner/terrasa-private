function roll(elementId) {
    return document.getElementById(elementId).textContent = elementId + ": " + Math.floor(Math.random() * 20 + 1);
}

function map_entities(f) {
    document.getElementsByClassName("entity").forEach(element => {
        f(element.elementId);
    });;
}
