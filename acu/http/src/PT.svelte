<script lang="ts">
  import Joystick, {
    DIRECTIONS as JoystickDirections,
  } from "./components/Joystick";

  function sendEvent(direction: JoystickDirections) {
    console.log(direction);
  }

  let dualControl = true;
</script>

<button class="button" on:click={() => (dualControl = !dualControl)}
  >Toggle Mode</button
>

{#if dualControl}
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
