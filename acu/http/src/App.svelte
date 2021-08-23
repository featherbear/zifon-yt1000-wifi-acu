<script lang="ts">
  import WiFi from "./WiFi.svelte";
  import PT from "./PT.svelte";

  import { onMount } from "svelte";
  import { fade } from "svelte/transition";

  let component;
  onMount(async () => {
    fetch("/mode")
      .then((r) => r.text())
      .then(
        (mode) =>
          (component = (function () {
            switch (mode) {
              case "config":
                return WiFi;
              default:
                return PT;
            }
          })())
      )
      .catch(() => {
        console.error("Could not fetch mode setting, reloading");
        setTimeout(() => {
          location.reload();
        }, 2000);
      });
  });
</script>

{#if component}
  <main in:fade>
    <svelte:component this={component} />
  </main>
{:else}
  <h2>Please wait...</h2>
{/if}

<style global lang="scss">
  @import "bulma/bulma.sass";
  * {
    user-select: none;
  }
  input {
    user-select: initial;
  }
  main {
    position: relative;
    max-width: 56em;
    background-color: white;
    padding: 2em;
    margin: 0 auto;
    box-sizing: border-box;
  }
</style>
