function roll(max, bonus) {
    return Math.floor(Math.random() * max) + 1 + parseInt(bonus);
}


function roll_initiative(element) {
    element.textContent = roll(20, element.textContent);
    element.classList.remove("initiative");
}


function map_entities(encounter, f) {
    var children = encounter.children;
    for (let index = 0; index < children.length; index++) {
        const element = children[index];
        if (element.classList.contains("initiative")) {
            console.log(element);
            f(element);
        }
        else {
            map_entities(element, f)
        }
    }
    // Sort the elements
}
