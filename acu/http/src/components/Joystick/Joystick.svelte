<script lang="ts">
  /**
   * Axis
   */
  export let type: "x" | "y" | "xy" = "xy";
  import Arrow from "./arrow-up-solid.svg";
  import { DIRECTIONS } from "./types";

  export let emitKeystream: (code: DIRECTIONS) => any = null;
  export let emitState: (state: boolean, code?: DIRECTIONS) => any = null;

  export let intervalRate = 25;

  let xAxisEnabled;
  let yAxisEnabled;

  let activeKeys = [];

  $: {
    xAxisEnabled = type.includes("x");
    yAxisEnabled = type.includes("y");
    if (activeKeys[0]) handleLoseFocus();
  }

  const KEYS = {
    37: DIRECTIONS.LEFT, // LEFT_ARROW
    38: DIRECTIONS.UP, // UP_ARROW
    39: DIRECTIONS.RIGHT, // RIGHT_ARROW
    40: DIRECTIONS.DOWN, // DOWN_ARROW
    65: DIRECTIONS.LEFT, // A
    68: DIRECTIONS.RIGHT, // D
    83: DIRECTIONS.DOWN, // S
    87: DIRECTIONS.UP, // W
  };

  function handleKeydown(evt: KeyboardEvent) {
    const code = evt.keyCode || evt.which;
    if (!KEYS.hasOwnProperty(code)) return;
    if (
      !xAxisEnabled &&
      [DIRECTIONS.LEFT, DIRECTIONS.RIGHT].includes(KEYS[code])
    )
      return;
    if (!yAxisEnabled && [DIRECTIONS.UP, DIRECTIONS.DOWN].includes(KEYS[code]))
      return;

    registerPress(code);
  }

  function handleKeyup(evt: KeyboardEvent) {
    const code = evt.keyCode || evt.which;
    if (!KEYS.hasOwnProperty(code)) return;
    if (
      !xAxisEnabled &&
      [DIRECTIONS.LEFT, DIRECTIONS.RIGHT].includes(KEYS[code])
    )
      return;
    if (!yAxisEnabled && [DIRECTIONS.UP, DIRECTIONS.DOWN].includes(KEYS[code]))
      return;

    unregisterPress(code);
  }

  function registerPress(key) {
    if (activeKeys[0] === key) return;
    activeKeys.unshift(key);
    emitState?.(true, KEYS[key] ?? key);
    checkLoop();
  }

  function unregisterPress(key) {
    let idx;
    let activeKey = activeKeys[0];

    if ((idx = activeKeys.indexOf(key)) == -1) return;
    activeKeys.splice(idx, 1);

    // See if there are other keys held down
    let nextActiveKey = activeKeys[0];
    if (activeKey !== nextActiveKey)
      emitState?.(
        nextActiveKey !== undefined,
        KEYS[nextActiveKey] ?? nextActiveKey
      );

    checkLoop();
  }
  function handleLoseFocus(evt?) {
    activeKeys = [];
    checkLoop();
  }

  let __eventLoopInterval = null;
  function checkLoop() {
    if (__eventLoopInterval) return;
    function fn() {
      let key = activeKeys[0];
      if (key === undefined) {
        clearInterval(__eventLoopInterval);
        __eventLoopInterval = null;
        emitState?.(false);
        return;
      }

      emitKeystream?.(KEYS[key] ?? key);
    }
    __eventLoopInterval = setInterval(fn, intervalRate);
    fn();
  }
</script>

<svelte:window
  on:keydown={handleKeydown}
  on:keyup={handleKeyup}
  on:blur={handleLoseFocus}
/>

<div class="ptControl">
  {#if yAxisEnabled}
    <div
      class="control controlUp"
      on:mousedown={() => registerPress(DIRECTIONS.UP)}
      on:mouseup={() => unregisterPress(DIRECTIONS.UP)}
    >
      <Arrow />
    </div>
    <div
      class="control controlDown"
      on:mousedown={() => registerPress(DIRECTIONS.DOWN)}
      on:mouseup={() => unregisterPress(DIRECTIONS.DOWN)}
    >
      <Arrow />
    </div>
  {/if}
  {#if xAxisEnabled}
    <div
      class="control controlLeft"
      on:mousedown={() => registerPress(DIRECTIONS.LEFT)}
      on:mouseup={() => unregisterPress(DIRECTIONS.LEFT)}
    >
      <Arrow />
    </div>
    <div
      class="control controlRight"
      on:mousedown={() => registerPress(DIRECTIONS.RIGHT)}
      on:mouseup={() => unregisterPress(DIRECTIONS.RIGHT)}
    >
      <Arrow />
    </div>
  {/if}
  <div class="controlOrigin"><slot /></div>
</div>

<style lang="scss">
  .ptControl {
    display: grid;
    grid-gap: 1px;

    grid-template-columns: repeat(3, 1fr);
    grid-template-rows: repeat(3, 1fr);

    .control {
      cursor: pointer;
      opacity: 0.6;
      transition: opacity 0.3s;

      &:hover {
        opacity: 0.9;
      }

      &.controlUp {
        grid-column: 2;
        grid-row: 1;
      }
      &.controlRight {
        grid-column: 3;
        grid-row: 2;
        transform: rotate(90deg);
      }
      &.controlDown {
        grid-column: 2;
        grid-row: 3;
        transform: rotate(180deg);
      }
      &.controlLeft {
        grid-column: 1;
        grid-row: 2;
        transform: rotate(270deg);
      }
    }

    .controlOrigin {
      grid-column: 2;
      grid-row: 2;

      display: flex;
      align-items: center;
      justify-content: center;
    }
  }
</style>
