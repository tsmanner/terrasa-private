---
entities:
  - name: Caltrop Bloodless
    bonus: 4
  - name: Gluteus Maximus
    bonus: 3
js:
  - /assets/js/d20.js
css:
  - /assets/css/encounter.css
---

Some stuff happens and then if they do a particular thing, Encounter 1 occurs!
<details>
  <summary>Test Encounter 1</summary>
  {% include encounter.html name="Encounter 1" %}
</details>

They do some other stuff, or take some other branch and end up at Encounter 2!

<details>
  <summary>Test Encounter 2</summary>
  {% include encounter.html name="Encounter 2" %}
</details>
