function roll(max, bonus) {
    return Math.floor(Math.random() * max) + 1 + parseInt(bonus);
}


function roll_initiative(element) {
    element.innerHTML = roll(20, element.innerHTML);
    element.classList.remove("initiative");
}


function map_rec(e, f) {
    var children = e.children;
    for (let index = 0; index < children.length; index++) {
        const element = children[index];
        if (element.classList.contains("initiative")) {
            console.log(element);
            f(element);
        }
        else {
            map_rec(element, f)
        }
    }
}


function map_entities(e, f) {
    map_rec(e, f);
    // Sort the elements
    var table = document.getElementById(e.id + ".table");
    var rows = []
    for (let i = 1; i < table.rows.length; i++) {
        rows.push(table.rows[i]);
    }
    rows.sort(
        function(lhs, rhs) {
            return parseInt(lhs.cells[1].innerHTML) - parseInt(rhs.cells[1].innerHTML);
        }
    );
    // Clear the table rows
    for (let i = 1; i < table.rows.length; i++) {
        table.rows.deleteRow(i);
    }
    // Insert the sorted rows
    for (let i = 0; i < rows.length; i++) {
        table.rows.insertRow(i+1);
    }
}
