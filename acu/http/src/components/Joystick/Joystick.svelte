<script lang="ts">
  /**
   * Axis
   */
  export let type: "x" | "y" | "xy" = "xy";
  import Arrow from "./arrow-up-solid.svg";
  import { DIRECTIONS } from "./types";

  export let emit: (code: DIRECTIONS) => any;

  export let intervalRate = 25;

  let xAxisEnabled;
  let yAxisEnabled;

  $: {
    xAxisEnabled = type.includes("x");
    yAxisEnabled = type.includes("y");
    handleLoseFocus();
  }

  let activeKeys = [];

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
    registerPress(code);
  }

  function registerPress(code) {
    if (activeKeys[0] === code) return;
    activeKeys.unshift(code);
    checkLoop();
  }

  function handleKeyup(evt: KeyboardEvent) {
    const code = evt.keyCode || evt.which;
    if (!KEYS.hasOwnProperty(code)) return;
    unregisterPress(code);
  }

  function unregisterPress(code) {
    let idx;
    if ((idx = activeKeys.indexOf(code)) == -1) return;
    activeKeys.splice(idx, 1);
    checkLoop();
  }
  function handleLoseFocus(evt?) {
    activeKeys = [];
    checkLoop();
  }

  let __eventLoopInterval = null;
  function checkLoop() {
    if (__eventLoopInterval) return;
    __eventLoopInterval = setInterval(function () {
      let key = activeKeys[0];
      if (key === undefined) {
        clearInterval(__eventLoopInterval);
        __eventLoopInterval = null;
        return;
      }

      emit?.(KEYS[key] ?? key);
    }, intervalRate);
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
