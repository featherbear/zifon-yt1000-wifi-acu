<script lang="ts">
  import Joystick, {
    DIRECTIONS as JoystickDirections,
  } from "./components/Joystick";

  let _sendEvent;
  function sendEvent(direction: JoystickDirections) {
    _sendEvent?.(direction);
  }

  let WS = new WebSocket(`ws://${location.hostname}:1337`);
  WS.addEventListener("open", () => {
    // show things are connected
    let lastVal;
    let lastBuf;

    function makeBuffer(val) {
      if (lastVal === val) return lastBuf;
      return (lastBuf = new Uint8Array([(lastVal = val)]));
    }

    _sendEvent = (direction: JoystickDirections) => {
      WS.send(makeBuffer(direction));
    };
  });

  let isDualControl = true;
</script>

<button class="button" on:click={() => (isDualControl = !isDualControl)}
  >Toggle Mode</button
>

{#if isDualControl}
  <div class="joystickContainer">
    <Joystick type="y" emit={sendEvent}>Y</Joystick>
  </div>

  <div class="joystickContainer">
    <Joystick type="x" emit={sendEvent}>X</Joystick>
  </div>
{:else}
  <div class="joystickContainer">
    <Joystick type="xy" emit={sendEvent}>XY</Joystick>
  </div>
{/if}

<style lang="scss">
  .joystickContainer {
    max-width: 20vw;
    max-height: 20vw;
  }
</style>
