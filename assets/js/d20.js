function roll(max, bonus) {
    return Math.floor(Math.random() * max) + 1 + parseInt(bonus);
}


function rollInitiative(element) {
    var children = element.children;
    for (let index = 0; index < children.length; index++) {
        var child = children[index];
        if (child.classList.contains("initiative-random")) {
            console.log(child);
            child.value = roll(20, child.innerHTML);
            child.innerText = child.value;
            child.classList.remove("initiative-random");
        }
        else {
            rollInitiative(child)
        }
    }
}


function inputInitiative(encounter, initiativeButtonId) {
    var value = prompt("Initiative:");
    if (value != null && value != "") {
        var initiative = document.getElementById(initiativeButtonId);
        initiative.value = value;
        initiative.innerText = value;
        initiative.classList.remove("initiative-random");
        sortEntityTable(encounter)
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
        row = rows[i];
    }
}
