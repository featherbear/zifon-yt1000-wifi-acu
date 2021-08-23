<script lang="ts">
  import { send } from "ionicons/icons";

  import Joystick, {
    DIRECTIONS as JoystickDirections,
  } from "./components/Joystick";

  function stateEvent(state: boolean, direction?: JoystickDirections) {
    if (state) updateBuffer(direction);
    else updateBuffer(0xff);
    _send();
  }

  let WS = new WebSocket(`ws://${location.hostname}:1337`);

  let lastVal;
  let buffer;
  function updateBuffer(val) {
    if (lastVal === val) return;
    buffer = new Uint8Array([(lastVal = val)]);
  }

  let _send = () => WS.send(buffer);
  WS.addEventListener("open", () => {
    // show things are connected
    setInterval(_send, 400);
  });

  let isDualControl = false;
</script>

<button class="button" on:click={() => (isDualControl = !isDualControl)}
  >Toggle Mode</button
>

{#if isDualControl}
  <div class="joystickContainer">
    <Joystick type="y" emitState={stateEvent}>Y</Joystick>
  </div>

  <div class="joystickContainer">
    <Joystick type="x" emitState={stateEvent}>X</Joystick>
  </div>
{:else}
  <div class="joystickContainer">
    <Joystick type="xy" emitState={stateEvent}>XY</Joystick>
  </div>
{/if}

<style lang="scss">
  .joystickContainer {
    max-width: 20vw;
    max-height: 20vw;
  }
</style>
