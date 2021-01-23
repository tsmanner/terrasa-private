---
encounters:
  - name: Encounter 1
    entities:
      - Brother Gromag
      - Caltrop Bloodless
      - Gluteus Maximus
      - Skeleton
      - Skeleton
  - name: Encounter 2
    entities:
      - Brother Gromag
      - Caltrop Bloodless
      - Skeleton
      - Skeleton
full-width: true
js:
  - /assets/js/d20.js
css:
  - /assets/css/encounter.css
---

Some stuff happens and then if they do a particular thing, Encounter 1 occurs!
{% include encounter.html name="Encounter 1" %}


They do some other stuff, or take some other branch and end up at Encounter 2!
{% include encounter.html name="Encounter 2" %}
