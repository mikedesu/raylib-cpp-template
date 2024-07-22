# TODO

- [ ] re-organize how music and sound effects are handled
- [ ] explore korg gadget for samples we can use
- [x] "properly" handle scene transitions
    - [x] need to be able to close a scene and then re-initialize it later
- [ ] move `draw()` and other type methods into sub-classes of `Scene`
    - [ ] this way we can define `draw()` on a per-scene basis and eliminate some of the hard-coding in `Scene::draw()`
    - [ ] TitleScene
    - [x] GameplayScene
    - [ ] GameOverScene
- [ ] expand the message popup system
- [ ] make bats spawn randomly from either side of the screen
    - [ ] also consider random y position
    - [ ] also consider adding a different way to handle movement
        - [ ] movement types
            - [ ] traditional vx/vy-based
            - [ ] "lock" onto player and move towards them
                - [ ] will have to calculate vx/vy every step to adjust direction
                - [ ] will work similar to item magnetism



