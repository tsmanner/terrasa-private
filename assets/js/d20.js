function roll(max, bonus) {
    return Math.floor(Math.random() * max) + 1 + parseInt(bonus);
}


function rollInitiative(element) {
    var children = element.children;
    for (let index = 0; index < children.length; index++) {
        var child = children[index];
        if (child.classList.contains("initiative") && child.classList.contains("randomize")) {
            console.log(child);
            child.value = roll(20, child.getAttribute("bonus"));
            child.innerText = child.value;
            child.classList.remove("randomize");
        }
        else {
            rollInitiative(child);
        }
    }
}


function updateValue(button, value) {

}


function resetValue(event, buttonId) {
    event.preventDefault();
    var button = document.getElementById(buttonId);
    var initialValue = button.getAttribute("initialValue");
    button.value = parseInt(initialValue);
    button.innerText = initialValue;
}


function inputValue(encounter, buttonId) {
    var initiative = document.getElementById(buttonId);
    var initiativeBonus = parseInt(initiative.getAttribute("initialValue"));
    var minimumValue = 1 + initiativeBonus;
    var maximumValue = 20 + initiativeBonus;
    var value = prompt("Initiative (range " + minimumValue + " to " + maximumValue + ")");
    if (minimumValue <= value && value <= maximumValue) {
        initiative.value = value;
        initiative.innerText = value;
    }
    else if (value != null) {
        inputValue(encounter, buttonId);
    }
}


function mapEntities(encounter, func) {
    func(encounter);
    sortEntityTable(encounter);
}


function sortEntityTable(encounter) {
    // Sort the elements
    var table = document.getElementById(encounter.id + ".table");
    var rows = []
    for (let i = 1; i < table.rows.length; i++) {
        rows.push(table.rows[i]);
    }
    rows.sort(function(lhs, rhs) {
        var d_init = parseInt(rhs.cells[1].children[0].value) - parseInt(lhs.cells[1].children[0].value);
        if (d_init == 0) {
            return parseInt(rhs.cells[3].innerHTML) - parseInt(lhs.cells[3].innerHTML);
        }
        return d_init;
    });
    // Clear the table rows
    var length = table.rows.length;
    for (let i = 1; i < length; i++) {
        table.deleteRow(1);
    }
    // Insert the sorted rows
    for (let i = 0; i < rows.length; i++) {
        row = table.insertRow(-1);
        row.innerHTML = rows[i].innerHTML;
    }
}

function onLoad() {
    var interactives = document.getElementsByClassName("interactive");
    for (let i = 0; i < interactives.length; i++) {
        interactives[i].addEventListener("onload", function(){ resetValue(event, interactives[i].id); })
    }
}
onLoad();
