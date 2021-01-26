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


function entityTableRowHasInitiativeValue(row) {
    let value = row.cells[1].children[0].dataset.value;
    return value != "null" && value != null;
}


function entityTableRowInitiativeValue(row) {
    return parseInt(row.cells[1].children[0].dataset.value);
}


function entityTableRowInitiativeModifier(row) {
    return parseInt(row.cells[1].children[0].dataset.modifier);
}


function entityTableRowDexModifier(row) {
    return parseInt(row.cells[5].innerHTML)
}


function entityTableRowCompare(lRow, rRow) {
    // Compare Priority
    //   Value
    //   if (values are equal) DEX Modifier
    //   if (no values) Initiative Bonus
    if (entityTableRowHasInitiativeValue(lRow) && entityTableRowHasInitiativeValue(rRow)) {
        let initiativeDiff = entityTableRowInitiativeValue(rRow) - entityTableRowInitiativeValue(lRow);
        if (initiativeDiff == 0) {
            return entityTableRowDexModifier(rRow) - entityTableRowDexModifier(lRow);
        }
        return initiativeDiff;
    }
    else if (entityTableRowHasInitiativeValue(lRow)) {
        return -1;  // Value vs No Value, -1 to put lRow first
    }
    else if (entityTableRowHasInitiativeValue(rRow)) {
        return 1;  // No Value vs Value, 1 to put rRow first
    }
    else {
        return entityTableRowInitiativeModifier(rRow) - entityTableRowInitiativeModifier(lRow);
    }
}


function sortEncounter(encounter) {
    // Sort the elements
    let table = document.getElementById(encounter.id + ".table");
    let rows = []
    for (let i = 1; i < table.rows.length; i++) {
        rows.push(table.rows[i]);
    }
    rows.sort(entityTableRowCompare);
    // Delete the existing rows
    let length = table.rows.length;
    for (let i = 1; i < length; i++) {
        table.deleteRow(-1);
    }
    // Insert the sorted rows
    length = rows.length;
    for (let i = 0; i < length; i++) {
        table.childNodes[1].appendChild(rows[i]);
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
    let identityWrapper = function (action, element) { return function () { action(element); } };
    let preventDefaultWrapper = function (action, element) { return function (event) { console.log(event); event.preventDefault(); action(element); } };
    let actions = {
        input: inputValue,
        roll: doRoll,
        reset: reset,
        increment: increment,
        decrement: decrement
    };
    let wrappers = {
        click: identityWrapper,
        auxclick: preventDefaultWrapper,
        contextmenu: preventDefaultWrapper
    };

    for (let actionName in actions) {
        let className = actionName + "-" + eventName;
        let action = actions[actionName];
        let elements = document.getElementsByClassName(className);
        for (let i = 0; i < elements.length; i++) { let element = elements[i];
            element.addEventListener(eventName, wrappers[eventName](action, element));
        }
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
    registerEventListeners("click");        // Left Click
    registerEventListeners("dblclick");     // Double Click
    registerEventListeners("auxclick");     // Middle Click
    registerEventListeners("contextmenu");  // Right Click

    // Sort all the encounter tables by initiative
    elements = document.getElementsByClassName("encounter");
    for (let i = 0; i < elements.length; i++) { let element = elements[i];
        sortEncounter(element);
    }

}


init();
