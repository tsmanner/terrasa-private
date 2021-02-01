# Javascript Organization

All classes are followed by


## Content Classes

### value
Public Attributes
  - data-value-initial
  - data-value-validator
Private Attributes
  - data-value
  - data-value-reference-ids

### value-range
Public Attributes
  - data-value-max
  - data-value-min
  - data-value-initial
Private Attributes
  - data-value
  - data-value-reference-ids : Space separated list of IDs

### value-reference
Public Attributes
  - data-value-source-id
  - data-value-source-attribute
Private Attributes
  - data-value

### value-sum
Public Attributes
  - data-value-sources : Space separated list of ID:Attribute pairs
Private Attributes
  - data-value

## Interaction Classes

### input
Public Attributes
  - data-input
  - data-input-attribute

### increment
Public Attributes
  - data-increment : Space separated list of event+keys to step to attribute `event[,key...]:step[:attribute]`

### decrement
Public Attributes
  - data-decrement-event
  - data-decrement-keys
  - data-decrement-step
  - data-decrement-attribute

### reset
Public Attributes
  - data-reset-event
  - data-reset-keys
  - data-reset-attribute
Private Attributes
  - data-reset-value : set at init to the reset-attribute's initial value

### roll
Public Attributes
  - data-roll-event
  - data-roll-keys
  - data-roll-attribute


## Rendering Classes

### render-value
Depends on Attributes
  - data-value

### render-modifier
Renders a value in modifier form
Depends on Attributes
  - data-value

### entity
