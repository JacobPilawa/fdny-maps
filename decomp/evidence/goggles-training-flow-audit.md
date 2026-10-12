# Training goggles flow audit

## Finding

Native Training does not currently select, update, or scan with goggles. The original control and goggles item exist in the preserved source, but the authored Training scripts do not grant the Player goggles. A follow-up static audit has now identified the item vtable, ACTION dispatch, and scan callback; see [`goggles-vtable-action-audit.md`](goggles-vtable-action-audit.md). Live reachability, control binding, visible results, and native implementation remain open.

## What the evidence proves

- Training entity 115 (RCS_PlayerRescue) authors equip axe, then loads doors.evs, sets ash density to 0, and disables RescueBot. It has no equip goggles command. The full decoded properties are in assets/training/entities.json; the authored scripts are in evidence/original/training.evs.
- The shared rescue.evs explicitly runs tell player (equip goggles) only under ifnot LEVEL training and difficulty [0, 1]. Thus that shared startup grant is excluded on Training. This proves the condition in the script, not whether another untraced runtime path grants goggles.
- The original Player-controls constructor 004395c0 registers a SELECT_GOGGLES control, then binds it with 00437410(..., 4). This is a distinct selection input from ACTION and from SELECT_AXE/SELECT_EXT. input.rrf defines fire, selectaxe, and selectext (including keypad aliases), but contains no selectgoggles binding. The actual default device/key behavior for code 4 in Training is therefore not established by this config alone.
- Original function 00462a60 creates item-like descriptor records with identifier 0x2bf and registers a UI/resource named Goggles at object offset +0x5dc. This supports the existence of a goggles object/resource; it does not by itself establish scan semantics.
- Original Player update 0044f340 dispatches ACTION pressed/held/released through item slots +0x14, +0x18, and +0x1c. The follow-up audit identifies the goggles object's +0x14 slot as 00462E80, which builds nearby actor target/effect records. That static class path does not show that goggles are equipped or selected in Training.

## Native status

In `src/main.cpp`, the script host now maps indexed item slots for the source-established Training Axe (slot 1) and Hose Spray (slot 4), and selection updates that inventory state. Extinguisher remains keyed to the source factory's item kind. There is still no goggles grant, goggles input, goggles item callback/state update, or scan action, so the native host cannot currently perform the goggles tutorial.

No goggles behavior was implemented from this static evidence. The Training grant, selection binding/default, live scan inputs/results, and consequences of pressing 1 after spotting a survivor are unresolved. In particular, the hint text in training.evs is not sufficient evidence for granting the item or for the live presentation of a scan.

## Next evidence needed

A controlled original Training trace should record (1) whether goggles are present in Player equipment at startup, (2) which input activates SELECT_GOGGLES, (3) whether selected item 3 reaches `00462E80` on ACTION press, and (4) the resulting survivor/HUD/message state while looking at and away from a target. The static vtable/ACTION map is in `goggles-vtable-action-audit.md`; implement the native flow only after its Training inputs and visible outcomes are observed.
