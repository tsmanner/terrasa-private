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
    var rows = table.getElementsByTagName("TR");
    rows.sort(
        function(lhs, rhs) {
            if (lhs.getElementsByTagName("TD")[1] == "Initiative") { return -1; }
            return parseInt(lhs.getElementsByTagName("TD")[1].innerHTML)
                 - parseInt(rhs.getElementsByTagName("TD")[1].innerHTML);
        }
    );
    // Clear the table rows
    for (let i = 0; i < table.rows.length; i++) {
        table.rows.deleteRow(i);
    }
    // Insert the sorted rows
    for (let i = 0; i < rows.length; i++) {
        table.rows.insertRow(i);
    }
}
