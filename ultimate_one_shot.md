---
title: Ultimate One Shot
subtitle: Rough script for my frisbee people one-shot
full-width: true

encounters:
  - name: Stuck in the mud
    entities:
      - dnd.monsters.srd."Bandit Captain"
      - dnd.monsters.srd."Bandit"
      - dnd.monsters.srd."Bandit"
      - dnd.monsters.srd."Bandit"
      - dnd.monsters.srd."Bandit"
  - name: The old inn
    entities:
      - dnd.monsters.srd."Bandit Captain"
      - dnd.monsters.srd."Skelton"
      - dnd.monsters.srd."Skeleton"
      - dnd.monsters.srd."Bandit"
      - dnd.monsters.srd."Bandit"
      - dnd.monsters.srd."Bandit"
      - dnd.monsters.srd."Bandit"

---

## Opening

Our story opens with you traveling south as part of a caravan of traders.  It's nearly dusk, the sun just starting to sink below the horizon over the peninsula to the west, and to the south you can just make out the lights of a city on the horizon.  You've been traveling with this Go'Val caravan for a few weeks now, some for the coin, some to follow a dream, and some to escape from your past.  They took you in without judgement, making you feel at home with them as you travel across the continent.

Suddenly, you hear a shout from near the front of the caravan, and watch as the lead wagon tilts precariously, nearly tipping over before it finally stops.  The rest of the wagons come to a halt, and as you all begin to gather, you notice that the ground here is really muddy from all the recent rain.

What do you do?


{% include dnd/encounter-head.html name="Stuck in the mud" %}

{% capture text %}
*Investigation or Perception 12 to notice that the road here was dug out a little bit.  Something is up.*

A man approaches from the side of the road:

**Man (Irish):** What a shame, to see another caravan stuck here.  Don't worry it happens all the time, I'm sure that me and my men could help you out.  *He shoots the caravan leader a wink.*  You let us decide what's fair for our trouble, and we'll make sure you get on your way, no harm, no foul.

If the players fight them, after two of the bandits are taken down, they all flee.
{% endcapture %}{{ text | markdownify }}

{% include dnd/encounter-body.html name="Stuck in the mud" %}

{% capture text %}
After the encounter with the highwaymen, you manage to get the wagons unstuck and continue on to the city.
{% endcapture %}{{ text | markdownify }}

{% include dnd/encounter-foot.html name="Stuck in the mud" %}


The city, Yalaqta, is a large port city, built across the mouth of a river that flows into the north end of a long bay.  The last few miles of road leading to it follow the banks of the river, both passing through the main gate.  Yalaqta is surrounded by rough mortarless stone walls rising about twenty feet.  The main gate is open, and you pass through, heading toward the market with the caravan.

**Maran:** Right, we'll get set up in an open space in the market.  For those of you that haven't been here before, we'll be heading down to the center of town, right near the banks of the river, to set up our wagons.  We'll be sleeping there, as we do on the road, but we're done with this leg of the journey.  We'll be here for a few days trading before we go on to Qullaqta.

*During the night, one of the children in the caravan goes missing.  Investigation checks!*
1-9: You don't see any signs of struggle etc near the wagons, but a short way away, you do see that some of the benches etc around the fire are knocked over and have been pushed around.
10-15: You find some marks on the ground near the knocked over bench that look like feet being dragged through the hard-packed dirt and mud.
16+: You follow the tracks to the side, and find a discarded toy near the entrance to an alleyway.

Investigation ensues, talking to the locals will yield information about a string of kidnappings.  1d4, based on the conversations...
1. Rumor has it that they're being taken by a witch that lives down near the docks.  I'm not sure I'm sold on that, but there is some shady stuff going on down there...
2. There's been something going on that old inn near the docks that burned down a few years ago.  I'd give that place a wiiiiide berth if I was you!
3. I heard, from my cousin of course, that there's a new act in town, someone I've only heard called "The King."  Now, listen here!  You didn't hear that from me!
4. I don't know nothin!

{% include dnd/encounter-head.html name="The old inn" %}

{% capture text %}

Old inn:
On the approach, you see an old wood and stone building with obvious scorch marks along the corners and sides, and some bits of collapsed in roof.  The windows are all boarded up, and a beggar(drawl) is sitting on the ground near the old front door.

Perception 12+: the dude is pretty well fed and fit... huh

*If you alert him in any way, he bangs on the door and you have to fight your way in/through all of them at once.*

*If you can get the jump on him, it's only him and the two inside the door.  The rest come after you enter.*


{% endcapture %}{{ text | markdownify }}

{% include dnd/encounter-body.html name="The old inn" %}

{% capture text %}

You go through the inn, and find several children, gagged and locked in the cellar.

{% endcapture %}{{ text | markdownify }}

{% include dnd/encounter-foot.html name="The old inn" %}

