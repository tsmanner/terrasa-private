function roll(max, bonus) {
    return Math.floor(Math.random() * max + 1 + bonus)
}


function roll_initiative(element) {
    return element.getElementById("initiative").textContent = roll(20, element.bonus);
}

function map_entities(f) {
    var entities = document.getElementsByClassName("entity");
    for (let index = 0; index < entities.length; index++) {
        const entity = entities[index];
        f(entity);
    }
}