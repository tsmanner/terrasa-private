function roll(max, bonus) {
    var r = Math.random() * max + 1;
    console.log("roll(" + max + ", " + bonus + ") = " + r + " + " + bonus + " = " + Math.floor(r + bonus));
    return Math.floor(r + bonus);
    return Math.floor(Math.random() * max + 1 + bonus);
}


function roll_initiative(element) {
    console.log(element.id + ".initiative = " + document.getElementById(element.id + ".initiative"));
    return document.getElementById(element.id + ".initiative").textContent = roll(20, element.getAttribute("initiative"));
}


function descendantOf(ancestorId, element) {
    if (element.parentElement == null) { return false; }
    else if (element.parentElement.id == ancestorId) { return true; }
    else { return descendantOf(ancestorId, element.parentElement); }
}


function map_entities(parent, f) {
    var entities = parent.children;
    for (let index = 0; index < entities.length; index++) {
        const element = entities[index];
        if (element.classList.contains("entity")) {
            console.log(element);
            f(element);
        }
    }
    // Sort the elements
}
