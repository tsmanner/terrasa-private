var abilityNames = ["str", "dex", "con", "int", "wis", "cha"];


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


function sortEncounter(tableId) {
    let table = document.getElementById(tableId);
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


function findSelectedRow(table) {
    for (let i = 1; i < table.rows.length; ++i) {
        if (table.rows[i].classList.contains("selected")) {
            return i;
        }
    }
    return 0;
}


function findNextRow(table, i) {
    for (let offset = 1; offset < table.rows.length - 1; ++offset) {
        let next = ((i - 1 + offset) % (table.rows.length - 1)) + 1;
        if (!table.rows[next].classList.contains("hidden")) {
            return next;
        }
    }
    return i;
}


function findPreviousRow(table, i) {
    for (let offset = 1; offset < table.rows.length - 1; ++offset) {
        let next = (i - 1 - offset + table.rows.length - 1) % (table.rows.length - 1) + 1;
        if (!table.rows[next].classList.contains("hidden")) {
            return next;
        }
    }
    return i;
}


function nextTurn(tableId) {
    let table = document.getElementById(tableId);
    let i = findSelectedRow(table);
    if (i > 0) {
        table.rows[i].classList.remove("selected");
        table.rows[findNextRow(table, i)].classList.add("selected");
    }
}


function previousTurn(tableId) {
    let table = document.getElementById(tableId);
    let i = findSelectedRow(table);
    if (i > 0) {
        table.rows[i].classList.remove("selected");
        table.rows[findPreviousRow(table, i)].classList.add("selected");
    }
}


function autoRollInitiative(tableId) {
    let table = document.getElementById(tableId);
    for (let i = 1; i < table.rows.length; ++i) {
        let row = table.rows[i];
        if (!entityTableRowHasInitiativeValue(row)) {
            doRoll(row.cells[1].children[0]);
        }
    }
    sortEncounter(tableId);
}


function resetEncounter(tableId) {
    showAll(tableId);
    let table = document.getElementById(tableId);
    table.rows[0].classList.remove("selected");
    for (let i = 1; i < table.rows.length; ++i) {
        let row = table.rows[i];
        reset(row.cells[1].children[0]);  // Initiative
        reset(row.cells[3].children[0]);  // HP
    }
    sortEncounter(tableId);
}


function showAll(tableId) {
    let table = document.getElementById(tableId);
    for (let i = 1; i < table.rows.length; ++i) {
        let row = table.rows[i];
        row.classList.remove("hidden");
        if (row.classList.contains("entity")) {
            row.cells[10].children[0].innerHTML = "Hide";
        }
    }
}


function showHideRow(rowId) {
    row = document.getElementById(rowId);
    if (row.classList.contains("selected")) {
        let tableId = row.dataset.encounterId + ".table";
        nextTurn(tableId);
    }
    row.classList.toggle("hidden");
    if (row.classList.contains("hidden")) {
        row.cells[10].children[0].innerHTML = "Show";
    }
    else {
        row.cells[10].children[0].innerHTML = "Hide";
    }
}


//
// Roll
//


function roll(max, mod) {
    return Math.floor(Math.random() * max) + 1 + parseInt(mod);
}


function getModifier(element) {
    return "modifier" in element.dataset ? parseInt(element.dataset.modifier) : 0;
}


function minimumRoll(element) {
    return 1 + getModifier(element);
}


function maximumRoll(element) {
    return parseInt(element.dataset.die) + getModifier(element);
}


function doRoll(element) {
    element.dataset.value = roll(element.dataset.die, getModifier(element));
    renderValue(element);
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
            renderValue(element);
        }
        else {
            inputValue(element);
        }
    }
}


function increment(element) {
    if (!("maximumValue" in element.dataset) || parseInt(element.dataset.value) < parseInt(element.dataset.maximumValue)) {
        element.dataset.value = parseInt(element.dataset.value) + 1;
        renderValue(element);
    }
}


function increment5(element) {
    let newValue = parseInt(element.dataset.value) + 5;
    if ("maximumValue" in element.dataset && newValue > parseInt(element.dataset.maximumValue)) {
        element.dataset.value = parseInt(element.dataset.maximumValue);
    }
    else {
        element.dataset.value = newValue;
    }
    renderValue(element);
}


function decrement(element) {
    if (!("minimumValue" in element.dataset) || parseInt(element.dataset.value) > parseInt(element.dataset.minimumValue)) {
        element.dataset.value = parseInt(element.dataset.value) - 1;
        renderValue(element);
    }
}


function decrement5(element) {
    let newValue = parseInt(element.dataset.value) - 5;
    if ("minimumValue" in element.dataset && newValue < parseInt(element.dataset.minimumValue)) {
        element.dataset.value = parseInt(element.dataset.minimumValue);
    }
    else {
        element.dataset.value = newValue;
    }
    renderValue(element);
}


//
// Ability
//


let RenderFunctions = {
    renderAbilityScore: function (element) {
        return document.getElementById(element.dataset.sourceId).dataset[element.dataset.sourceAttribute];
    },
    renderAbilityMod: function (element) {
        return toStringWithSign(modifier(document.getElementById(element.dataset.sourceId).dataset[element.dataset.sourceAttribute]));
    },
    renderAbilitySave: function (element) {
        let entity = document.getElementById(element.dataset.sourceId);
        let save = modifier(entity.dataset[element.dataset.sourceAttribute]);
        if ((element.dataset.ability + "Bonuses") in entity.dataset) {
            let bonuses = entity.dataset[element.dataset.ability + "Bonuses"].split(" ");
            for (let i in bonuses) {
                save += parseInt(entity.dataset[bonuses[i]]);
            }
        }
        return toStringWithSign(save);
    }
}


//
// API Functions
//


function update(element) {
    if (element.classList.contains("entity")) {
        let hpElement = element.cells[3].children[0];
        let currentHp = parseInt(hpElement.dataset.value);
        let maxHp = parseInt(hpElement.dataset.maximumValue);
        if (currentHp == 0) {
            element.classList.remove("healthy", "bloodied");
            element.classList.add("unconscious");
        }
        else if (currentHp <= (maxHp / 2)) {
            element.classList.remove("unconscious", "healthy");
            element.classList.add("bloodied");
        }
        else {
            element.classList.remove("unconscious", "bloodied");
            element.classList.add("healthy");
        }
    }
}


function renderValue(element) {
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
    if ("cascadeUpdate" in element.dataset) {
        let cascades = element.dataset.cascadeUpdate.split(" ");
        for (let i in cascades) {
            let cascade = cascades[i];
            update(document.getElementById(cascade));
        }
    }
}


function renderFunction(element) {
    element.innerHTML = RenderFunctions[element.dataset.format](element);
}


function maximum(element) {
    if (element.classList.contains("roll")) {
        return maximumRoll(element);
    }
    else if (element.classList.contains("value-range")) {
        return element.dataset.maximumValue;
    }
}


function minimum(element) {
    if (element.classList.contains("roll")) {
        return minimumRoll(element);
    }
    else if (element.classList.contains("value-range")) {
        return element.dataset.minimumValue
    }
}


function reset(element) {
    element.dataset.value = element.dataset.initialValue;
    renderValue(element);
}


function modifier(score) {
    return Math.floor((score - 10) / 2);
};


function toStringWithSign(value) {
    if (parseInt(value) >= 0) {
        return "+" + value;
    }
    return value;
}


function setAbilityFormat(abilityFormat) {
    elements = document.getElementsByClassName("ability");
    for (let i = 0; i < elements.length; ++i) { let element = elements[i];
        element.dataset.format = abilityFormat;
        renderFunction(element);
    }
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
        increment5: increment5,
        decrement: decrement,
        decrement5: decrement5
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


function initEntityAbilityData(entity, abilityName) {
    entity.dataset[abilityName + "Mod"] = modifier(parseInt(entity.dataset[abilityName]));
    entity.dataset[abilityName + "Save"] = parseInt(entity.dataset[abilityName + "Mod"]) + parseInt(entity.dataset.proficiency);
}


function init() {
    let elements = [];

    // Initialize Entity mod and save data
    elements = document.getElementsByClassName("entity");
    for (let i = 0; i < elements.length; ++i) { let element = elements[i];
        abilityNames.map(function (abilityName) { initEntityAbilityData(element, abilityName); });
    }

    // Initialize 'roll' and 'value-range' instances
    elements = document.getElementsByClassName("roll");
    for (let i = 0; i < elements.length; ++i) { let element = elements[i];
        element.dataset.initialValue = null;
        reset(element);
    }
    elements = document.getElementsByClassName("value-range");
    for (let i = 0; i < elements.length; ++i) { let element = elements[i];
        reset(element);
    }
    // Register Event Listeners
    registerEventListeners("click");        // Left Click
    registerEventListeners("dblclick");     // Double Click
    registerEventListeners("contextmenu");  // Right Click

    setAbilityFormat("renderAbilityMod");

    elements = document.getElementsByClassName("initiative");
    for (let i = 0; i < elements.length; ++i) { let element = elements[i];
        let entity = document.getElementById(element.dataset.entityId);
        element.dataset.value = null;
        let initiativeModifier = parseInt(entity.dataset["dexMod"]);
        if ("initiativeBonuses" in entity.dataset) {
            let bonuses = entity.dataset.initiativeBonuses.split(" ");
            for (let i in bonuses) {
                initiativeModifier += parseInt(entity.dataset[bonuses[i]]);
            }
        }
        element.dataset.modifier = initiativeModifier;
        element.dataset.nullFormat = "+{modifier}";
        element.dataset.format = "{value}";
        renderValue(element);
    }

    // Sort all the encounter tables by initiative
    elements = document.getElementsByClassName("encounter");
    for (let i = 0; i < elements.length; ++i) { let element = elements[i];
        let tableId = element.id + ".table";
        sortEncounter(tableId);
    }

}


init();
