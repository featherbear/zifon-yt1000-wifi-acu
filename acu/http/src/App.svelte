<script>
  import WiFi from "./WiFi.svelte";
  import PT from "./PT.svelte";
</script>

<main>
  <svelte:component this={PT} />
</main>

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
