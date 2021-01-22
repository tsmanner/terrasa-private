function roll(max, bonus) {
    console.log("roll(" + max + ", " + bonus + ")")
    return Math.floor(Math.random() * max + 1 + bonus)
}


function roll_initiative(element) {
    console.log(element.id + ".initiative = " + document.getElementById(element.id + ".initiative"));
    return document.getElementById(element.id + ".initiative").textContent = roll(20, element.getAttribute("bonus"));
}

function map_entities(f) {
    var entities = document.getElementsByClassName("entity");
    for (let index = 0; index < entities.length; index++) {
        const element = entities[index];
        console.log(element);
        f(element);
    }
}
