function roll(elementId) {
    return document.getElementById(elementId).textContent = elementId + ": " + Math.floor(Math.random() * 20 + 1);
}

function map_entities(f) {
    var entities = document.getElementsByClassName("entity");
    for (let index = 0; index < entities.length; index++) {
        const entity = entities[index];
        f(entity.elementId);
    }
}
