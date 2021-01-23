function roll(max, bonus) {
    return Math.floor(Math.random() * max) + 1 + parseInt(bonus);
}


function roll_initiative(e) {
    var children = e.children;
    for (let index = 0; index < children.length; index++) {
        const element = children[index];
        if (element.classList.contains("initiative")) {
            console.log(element);
            element.innerHTML = roll(20, element.innerHTML);
            element.classList.remove("initiative");
        }
        else {
            roll_initiative(element, f)
        }
    }
}


function map_entities(e, func) {
    func(e);
    // Sort the elements
    var table = document.getElementById(e.id + ".table");
    var rows = []
    for (let i = 1; i < table.rows.length; i++) {
        rows.push(table.rows[i]);
    }
    rows.sort(function(lhs, rhs) {
        var d_init = parseInt(rhs.cells[1].innerHTML) - parseInt(lhs.cells[1].innerHTML);
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
