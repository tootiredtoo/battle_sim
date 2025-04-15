Concept

Simulate a battle between multiple teams of fighters (e.g., RPG or fantasy setting). 

Each fighter runs in its own thread, computes its action (attack, heal, defend), 
and synchronizes with others before the next turn. 

The system uses futures/promises to coordinate turns and resolve actions, ensuring thread-safe combat updates.
