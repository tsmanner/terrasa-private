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


function sortEncounter(encounter) {
    // Sort the elements
    let table = document.getElementById(encounter.id + ".table");
    let rows = []
    for (let i = 1; i < table.rows.length; i++) {
        rows.push(table.rows[i]);
    }
    rows.sort(function(lhs, rhs) {
        let initiativeDiff = parseInt(rhs.cells[1].children[0].dataset.value)
                           - parseInt(lhs.cells[1].children[0].dataset.value);
        if (initiativeDiff == 0) {
            return parseInt(rhs.cells[5].innerHTML) - parseInt(lhs.cells[5].innerHTML);
        }
        return initiativeDiff;
    });
    // Replace the table rows with the sorted ones
    let length = table.rows.length;
    for (let i = 1; i < length; i++) {
        table.rows[i].replaceWith(rows[i-1]);
    }
}


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


function inputValue(element) {
    let value = prompt(element.id + " (range " + minimum(element) + " to " + maximum(element) + ")");
    if (value != null) {
        value = parseInt(value);
        if (minimum(element) <= value && value <= maximum(element)) {
            element.dataset.value = value;
            render(element);
        }
        else {
            inputValue(element);
        }
    }
}


function increment(element) {
    if (!("maximumValue" in element.dataset) || parseInt(element.dataset.value) < parseInt(element.dataset.maximumValue)) {
        element.dataset.value = parseInt(element.dataset.value) + 1;
        render(element);
    }
}

function decrement(element) {
    if (!("minimumValue" in element.dataset) || parseInt(element.dataset.value) > parseInt(element.dataset.minimumValue)) {
        element.dataset.value = parseInt(element.dataset.value) - 1;
        render(element);
    }
}


//
// API Functions
//


function render(element) {
    let result = "{value}";
    if (element.dataset.value == null || element.dataset.value == "null") {
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


function registerEventListeners(eventName) {
    elements = document.getElementsByClassName("input-" + eventName);
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener(eventName, function (event) { event.preventDefault(); inputValue(element); });
    }

    elements = document.getElementsByClassName("roll roll-" + eventName);
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener(eventName, function (event) { event.preventDefault(); doRoll(element); });
    }

    elements = document.getElementsByClassName("reset-" + eventName);
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener(eventName, function (event) { event.preventDefault(); reset(element); });
        if (!"initialValue" in element.dataset) { element.dataset.initialValue = null; }
    }

    elements = document.getElementsByClassName("increment-" + eventName);
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener(eventName, function (event) { event.preventDefault(); increment(element); });
    }

    elements = document.getElementsByClassName("decrement-" + eventName);
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.addEventListener(eventName, function (event) { event.preventDefault(); decrement(element); });
    }

}


function init() {
    let elements = [];
    // Initialize 'roll' and 'value' instances
    elements = document.getElementsByClassName("roll");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        element.dataset.initialValue = null;
        reset(element);
    }
    elements = document.getElementsByClassName("value");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        reset(element);
    }
    // Register Event Listeners
    registerEventListeners("click");
    registerEventListeners("dblclick");
    registerEventListeners("contextmenu");

}


init();
