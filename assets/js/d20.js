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
    return element.hasAttribute("modifier") ? parseInt(element.getAttribute("modifier")) : 0;
}


function minimumRoll(element) {
    return 1 + modifier(element);
}


function maximumRoll(element) {
    return parseInt(button.getAttribute("die")) + modifier(element);
}


function resetRoll(element) {
    element.setAttribute("value", null);
}


function doRoll(element) {
    element.setAttribute("value", roll(element.getAttribute("die"), modifier(element)));
    render(element);
}


function renderRoll(element) {
    let body = "";
    if (element.getAttribute("value") == null || element.getAttribute("value") == "null") {
        if (element.classList.contains("modified-roll")) {
            let mod = modifier(element);
            body = (mod >= 0 ? "+" : "") + mod;
        }
        else {
            body = "d" + element.getAttribute("die");
        }
    }
    else {
        body = element.getAttribute("value");
    }
    return body;
}


//
// Modified Roll
//


//
// Value
//


function maximumValue(element) {
    return element.getAttribute("maximumValue");
}


function minimumValue(element) {
    return element.getAttribute("minimumValue");
}


function resetValue(element) {
    element.setAttribute("value", element.getAttribute("initialValue"));
}


function renderValue(element) {
    let body = element.getAttribute("value");
    if (element.getAttribute("showTotal")) {
        body += "/" + maximumValue(element);
    }
    return body;
}

//
// API Functions
//


function render(element) {
    let prefix = element.hasAttribute("prefix") ? element.getAttribute("prefix") : "";
    let suffix = element.hasAttribute("suffix") ? element.getAttribute("suffix") : "";
    let renderer = null;
    if (element.classList.contains("roll") || element.classList.contains("modified-roll")) {
        renderer = renderRoll;
    }
    else if (element.classList.contains("value")) {
        renderer = renderValue;
    }
    element.innerHTML = prefix + renderer(element) + suffix;
}


function maximum(element) {
    if (element.classList.contains("roll") || element.classList.contains("modified-roll")) {
        return maximumRoll(element);
    }
    else if (element.classList.contains("value")) {
        return maximumValue(element);
    }
}


function reset(element) {
    if (element.classList.contains("roll") || element.classList.contains("modified-roll")) {
        resetRoll(element);
    }
    else if (element.classList.contains("value")) {
        resetValue(element);
    }
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
    //     - modified-roll
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
        reset(element);
    }
    elements = document.getElementsByClassName("modified-roll");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("load", function () { reset(element); });
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
    console.log("Roll-Clicks:", elements);
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener("click", function () { doRoll(element); });
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
        element.setAttribute("initialValue", element.innerHTML);
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
