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
    for (let i = 1; i < table.rows.length; ++i) {
        table.rows[i].classList.remove("selected");
        rows.push(table.rows[i]);
    }
    rows.sort(entityTableRowCompare);
    // Delete the existing rows
    let length = table.rows.length;
    for (let i = 1; i < length; ++i) {
        table.deleteRow(-1);
    }
    // Insert the sorted rows
    length = rows.length;
    for (let i = 0; i < length; ++i) {
        table.childNodes[1].appendChild(rows[i]);
    }
    table.rows[1].classList.add("selected");
}


function nextTurn(encounter) {
    let table = document.getElementsByName(encounter.id + ".table");
    for (let i = 1; i < table.rows.length; ++i) {
        if (table.rows[i].classList.contains("selected")) {
            table.rows[i].classList.remove("selected");
            let nextI = i + 1;
            if (nextI >= table.rows.length) { nextI = 1; }
            table.rows[nextI].classList.add("selected");
        }
    }
}


function previousTurn(encounter) {
    let table = document.getElementsByName(encounter.id + ".table");
    for (let i = 1; i < table.rows.length; ++i) {
        if (table.rows[i].classList.contains("selected")) {
            table.rows[i].classList.remove("selected");
            let previousI = i - 1;
            if (previousI <= 0) { previousI = table.rows.length - 1; }
            table.rows[previousI].classList.add("selected");
        }
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
    let preventDefaultWrapper = function (action, element) { return function (event) { event.preventDefault(); action(element); } };

    let actions = {
        input: inputValue,
        roll: doRoll,
        reset: reset,
        increment: increment,
        decrement: decrement
    };
    let wrappers = {
        click: identityWrapper,
        dblclick: identityWrapper,
        contextmenu: preventDefaultWrapper
    };
    let predicates = {
        noMod: function (f) { return function (event) { if (!(event.shiftKey || event.ctrlKey || event.altKey)) { f(event) } }; },
        shift: function (f) { return function (event) { if (event.shiftKey) { f(event); } }; },
        ctrl: function (f) { return function (event) { if (event.ctrlKey) { f(event); } }; },
        alt: function (f) { return function (event) { if (event.altKey) { f(event); } }; }
    }

    for (let actionName in actions) {
        // Unpredicated Events
        let className = actionName + "-" + eventName;
        let action = actions[actionName];
        let elements = document.getElementsByClassName(className);
        for (let i = 0; i < elements.length; ++i) { let element = elements[i];
            element.addEventListener(eventName, predicates.noMod(wrappers[eventName](action, element)));
        }
        // Predicated Events
        for (let key in predicates) {
            className = actionName + "-" + key + "-" + eventName;
            action = actions[actionName];
            elements = document.getElementsByClassName(className);
            for (let i = 0; i < elements.length; ++i) { let element = elements[i];
                let listener = predicates[key](wrappers[eventName](action, element));
                element.addEventListener(eventName, listener);
            }
        }
    }
}


function init() {
    let elements = [];
    // Initialize 'roll' and 'value' instances
    elements = document.getElementsByClassName("roll");
    for (let i = 0; i < elements.length; ++i) { let element = elements[i];
        element.dataset.initialValue = null;
        reset(element);
    }
    elements = document.getElementsByClassName("value");
    for (let i = 0; i < elements.length; ++i) { let element = elements[i];
        reset(element);
    }
    // Register Event Listeners
    registerEventListeners("click");        // Left Click
    registerEventListeners("dblclick");     // Double Click
    registerEventListeners("contextmenu");  // Right Click

    // Sort all the encounter tables by initiative
    elements = document.getElementsByClassName("encounter");
    for (let i = 0; i < elements.length; ++i) { let element = elements[i];
        sortEncounter(element);
    }

}


init();
