// function rollInitiative(element) {
//     let children = element.children;
//     for (let index = 0; index < children.length; index++) {
//         let child = children[index];
//         if (child.classList.contains("initiative") && child.classList.contains("randomize")) {
//             child.value = roll(20, child.getAttribute("bonus"));
//             child.innerText = child.value;
//             child.classList.remove("randomize");
//         }
//         else {
//             rollInitiative(child);
//         }
//     }
// }


// function doRoll(element, value) {
//     if (minimumRoll <= value && value <= maximumRoll) {
//         element.value = value;
//         if (element.getAttribute("showMax")) {
//             element.innerText = value + "/" + maximumRoll;
//         }
//         else {
//             element.innerText = value;
//         }
//         return true;
//     }
//     else {
//         return false;
//     }
// }


// function updateValue(element, value) {
//     let minimumRoll = parseInt(element.getAttribute("minimumRoll"));
//     let maximumRoll = parseInt(element.getAttribute("maximumRoll"));
//     if (minimumRoll <= value && value <= maximumRoll) {
//     }
// }


// function incrementValue(element) {
//     updateValue(element, parseInt(element.value) + 1);
// }


// function decrementValue(element) {
//     updateValue(element, parseInt(element.value) - 1);
// }


// function resetModifiedRoll(element) {
//     let mod = modifier(element);
//     element.setAttribute("value", null);
//     element.innerText = (mod >= 0 ? "+" : "") + mod;
// }


// function resetValue(element) {
//     element.setAttribute("value", element.getAttribute("initialValue"));
//     element.innerHTML = element.getAttribute("initialValue");
// }


// function minimum(element) {
//     if (element.classList.contains("roll"))
// }


// function inputValue(encounter, element) {
//     let value = prompt(element.getAttribute("prompt") + " (range " + minimum(element) + " to " + maximum(element) + ")");
//     if (value != null && !updateValue(element, value)) {
//         inputValue(encounter, element);
//     }
// }


// function mapEntities(encounter, func) {
//     func(encounter);
//     sortEntityTable(encounter);
// }


// function sortEntityTable(encounter) {
//     // Sort the elements
//     let table = document.getElementById(encounter.id + ".table");
//     let rows = []
//     for (let i = 1; i < table.rows.length; i++) {
//         rows.push(table.rows[i]);
//     }
//     rows.sort(function(lhs, rhs) {
//         let d_init = parseInt(rhs.cells[1].children[0].value) - parseInt(lhs.cells[1].children[0].value);
//         if (d_init == 0) {
//             return parseInt(rhs.cells[3].innerHTML) - parseInt(lhs.cells[3].innerHTML);
//         }
//         return d_init;
//     });
//     // Clear the table rows
//     let length = table.rows.length;
//     for (let i = 1; i < length; i++) {
//         table.deleteRow(1);
//     }
//     // Insert the sorted rows
//     for (let i = 0; i < rows.length; i++) {
//         row = table.insertRow(-1);
//         row.innerHTML = rows[i].innerHTML;
//     }
// }




























//
// Roll
//


function roll(max, mod) {
    return Math.floor(Math.random() * max) + 1 + parseInt(mod);
}


function modifier(element) {
    return "modifier" in element.dataset ? parseInt(element.dataset.modifier) : 0;
}


function minimumRoll(element) {
    return 1 + modifier(element);
}


function maximumRoll(element) {
    return parseInt(element.dataset.die) + modifier(element);
}


function doRoll(element) {
    element.dataset.value = roll(element.dataset.die, modifier(element));
    render(element);
}


//
// Value
//

function increment(element) {
    element.dataset.value = parseInt(element.dataset.value) + 1;
    render(element);
}

function decrement(element) {
    element.dataset.value = parseInt(element.dataset.value) - 1;
    render(element);
}


//
// API Functions
//


function render(element) {
    let result = "{value}";
    if (element.dataset.value in [null, "null"]) {
        if ("nullFormat" in element.dataset) {
            result = element.dataset.nullFormat;
        }
    }
    else if ("format" in element.dataset) {
        result = element.dataset.format;
    }
    for (let k in element.dataset) {
        result = result.replace(new RegExp("{"+k+"}", "g"), element.dataset[k]);
    }
    element.innerHTML = result;
}


function maximum(element) {
    if (element.classList.contains("roll")) {
        return maximumRoll(element);
    }
    else if (element.classList.contains("value")) {
        return element.dataset.maximumValue;
    }
}


function minimum(element) {
    if (element.classList.contains("roll")) {
        return minimumRoll(element);
    }
    else if (element.classList.contains("value")) {
        return element.dataset.minimumValue
    }
}


function reset(element) {
    element.dataset.value = element.dataset.initialValue;
    render(element);
}


//
// Initialization
//

function init() {
    let elements = [];
    // Classes
    //   Value Types
    //     - roll
    //     - value
    //   Value Modifiers
    //     - roll-button
    //     - roll-remaining-button
    //     - input-click
    //     - increment-click
    //     - decrement-click
    //     - reset-click
    //     - roll-click
    //     - input-contextmenu
    //     - roll-contextmenu
    //     - increment-contextmenu
    //     - decrement-contextmenu
    //     - reset-contextmenu

    //
    // Initialization (onload)
    //

    elements = document.getElementsByClassName("roll");
    console.log("Rolls:", elements);
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("load", function () { reset(element); });
        element.dataset.initialValue = null;
        reset(element);
    }
    elements = document.getElementsByClassName("value");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("load", function () { reset(element); });
        reset(element);
    }

    //
    // Left Click (onclick)
    //

    elements = document.getElementsByClassName("input-click");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("click", function () { inputValue(element); });
    }

    elements = document.getElementsByClassName("roll-click");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("click", function () { doRoll(element); });
    }

    elements = document.getElementsByClassName("increment-click");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("click", function () { increment(element); });
    }

    elements = document.getElementsByClassName("decrement-click");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("click", function () { decrement(element); });
    }

    //
    // Right Click (contextmenu)
    //

    elements = document.getElementsByClassName("roll-contextmenu");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("contextmenu", function (event) { event.preventDefault(); doRoll(element); });
    }

    elements = document.getElementsByClassName("reset-contextmenu");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("contextmenu", function (event) { event.preventDefault(); reset(element); });
        if (!"initialValue" in element.dataset) {
            element.dataset.initialValue = null;
        }
    }

    elements = document.getElementsByClassName("increment-contextmenu");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("contextmenu", function (event) { event.preventDefault(); increment(element); });
    }

    elements = document.getElementsByClassName("decrement-contextmenu");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("contextmenu", function (event) { event.preventDefault(); decrement(element); });
    }










    // let elements = document.getElementsByClassName("resettable modifier");
    // for (let i = 0; i < elements.length; i++) { let element = elements[i];
    //     element.addEventListener("load",        function (event) { resetModifiedRoll(element); });
    //     element.addEventListener("contextmenu", function (event) { event.preventDefault(); resetModifiedRoll(element); });
    // }

    // let elements = [...document.getElementsByClassName("roll")].filter(function (element) { !element.classList.contains("modifier") });
    // for (let i = 0; i < elements.length; i++) { let element = elements[i];
    //     element.addEventListener("load",        function (event) { resetRoll(element); });
    //     element.addEventListener("contextmenu", function (event) { event.preventDefault(); resetRoll(element); });
    // }

    // let elements = document.getElementsByClassName("user-input");
    // for (let i = 0; i < elements.length; i++) { let element = elements[i];
    //     element.addEventListener("click", function (event) { inputValue(element); });
    // }

    // let elements = document.getElementsByClassName("counter");
    // for (let i = 0; i < elements.length; i++) { let element = elements[i];
    //     element.addEventListener("load",       function (event) { resetCounter(element); });
    // }

    // let elements = document.getElementsByClassName("counter up");
    // for (let i = 0; i < elements.length; i++) { let element = elements[i];
    //     element.addEventListener("click",       function (event) { incrementValue(element); });
    //     element.addEventListener("contextmenu", function (event) { event.preventDefault(); decrementValue(element); });
    // }

    // let elements = document.getElementsByClassName("counter down");
    // for (let i = 0; i < elements.length; i++) { let element = elements[i];
    //     element.addEventListener("click",       function (event) { decrementValue(element); });
    //     element.addEventListener("contextmenu", function (event) { event.preventDefault(); incrementValue(element); });
    // }

}


init();
