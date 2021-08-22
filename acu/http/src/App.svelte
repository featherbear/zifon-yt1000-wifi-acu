<script lang="ts">
  let showPasswordAsText = false;
  let useStatic = false;
  let countdownTimer = 60;

  let passwordElem;
  let successModalElem;

  let isSubmitting = false;

  let ssid = "";
  let password = "";

  let staticIP = "";
  let staticMask = "";

  import { eyeOffOutline, eye } from "ionicons/icons/index";
  import "bulma-checkradio";

  import IPAddressMask from "./components/IPAddressMask";

  function doSave() {
    isSubmitting = true;

    let ipData: { mode: string; static_ip?: string; static_mask?: string } = {
      mode: "dhcp", // Default
    };

    if (useStatic) {
      ipData = {
        mode: "static",
        static_ip: staticIP,
        static_mask: staticMask,
      };
    }

    const body = new URLSearchParams();
    for (let [key, val] of Object.entries({
      ssid,
      password,
      ...ipData,
    }))
      body.append(key, val.toString());

    fetch("/edit", {
      method: "POST",
      body,
    })
      .then((r) => {
        successModalElem.classList.add("is-active");
        let interval = setInterval(() => {
          if (--countdownTimer == 0) {
            clearInterval(interval);
            window.location.reload();
          }
        }, 1000);
      })
      .finally(() => {
        isSubmitting = false;
      });
  }

  type NetworkResult = Array<[string, string | string[], number]>;
  let networkResult: NetworkResult = [];
  let networkRequest_fetchPromise: Promise<NetworkResult>;

  async function doUpdateData() {
    if (networkRequest_fetchPromise) return;

    networkRequest_fetchPromise = fetch("/data/networks")
      .then((r) => r.json())
      .then((j: NetworkResult) => {
        let mapping: {
          [ssid: string]: {
            bssid: string[];
            rssi: number;
          };
        } = {};

        for (let [ssid, bssid, rssi] of j) {
          if (!mapping.hasOwnProperty(ssid)) {
            mapping[ssid] = { bssid: [bssid as string], rssi };
            continue;
          }

          mapping[ssid]["bssid"].push(bssid as string);
          if (rssi > mapping[ssid]["rssi"]) mapping[ssid]["rssi"] = rssi;
        }

        let result: NetworkResult = [];
        for (let ssid in mapping)
          result.push([ssid, mapping[ssid]["bssid"], mapping[ssid]["rssi"]]);
        return result;
      })
      .then((d) => (networkResult = d))
      .finally(() => {
        networkRequest_fetchPromise = null;
      });
  }

  import { onMount } from "svelte";
  let ready = false;
  onMount(() => {
    doUpdateData();
    fetch("/data/current")
      .then((r) => r.json())
      .then((j) => {
        ssid = j["ssid"];
        password = j["password"];
        useStatic = j["mode"] != "dhcp";
        staticIP = j["static_ip"];
        staticMask = j["static_mask"];
        ready = true;
      });
    setInterval(() => doUpdateData(), 5000);
  });
</script>

<svelte:head>
  <title>PT Config</title>
</svelte:head>

<main>
  {#if ready}
    <h1 class="title has-text-centered">PT Config</h1>
    <form>
      <div class="label">Detected networks</div>
      <ul id="networksList">
        {#each networkResult as [SSID, BSSID, RSSI] (SSID)}
          <li
            class:is-active={ssid == SSID}
            on:click={() => {
              if (ssid != SSID) password = "";
              ssid = SSID;
            }}
          >
            {SSID}<subtitle>({RSSI} dBm)</subtitle>
          </li>
        {/each}
      </ul>
    </form>

    <form>
      <div class="field">
        <div class="label">SSID</div>
        <div class="control">
          <input
            class="input"
            name="ssid"
            type="text"
            bind:value={ssid}
            required
            placeholder="Enter SSID"
          />
        </div>
      </div>

      <div class="field">
        <div class="label">Password</div>
        <div class="field has-addons">
          <div class="control" style="flex: 1">
            <input
              name="password"
              width="100%"
              class="input"
              bind:value={password}
              type="password"
              placeholder="Enter password"
              bind:this={passwordElem}
            />
          </div>
          <div class="control">
            <div
              on:click={() =>
                (passwordElem.type = (showPasswordAsText = !showPasswordAsText)
                  ? "text"
                  : "password")}
              class="button is-info"
            >
              <span class="icon">
                <img
                  src={showPasswordAsText ? eye : eyeOffOutline}
                  alt="Password visibility toggle"
                />
              </span>
            </div>
          </div>
        </div>
      </div>
    </form>

    <form on:submit|preventDefault>
      <div class="field">
        <div class="label">Select IP Configuration</div>
        <div class="field">
          <input
            class="is-checkradio is-info"
            id="IPuseDHCP"
            type="radio"
            name="IPuseDHCPorStatic"
            checked={!useStatic}
            on:change={() => (useStatic = false)}
          />
          <label for="IPuseDHCP">DHCP</label>
          <input
            class="is-checkradio is-info"
            id="IPuseStatic"
            type="radio"
            name="IPuseDHCPorStatic"
            checked={useStatic}
            on:change={() => (useStatic = true)}
          />
          <label for="IPuseStatic">Static</label>
        </div>
      </div>

      <div>
        <div class="field">
          <div class="label">IP Address</div>
          <div class="control">
            <input
              class="input"
              name="static_ip"
              type="text"
              use:IPAddressMask
              disabled={!useStatic}
              bind:value={staticIP}
              placeholder="Enter IP Address as xxx.xxx.xxx.xxx"
            />
          </div>
        </div>
        <div class="field">
          <div class="label">Subnet</div>
          <div class="control">
            <input
              class="input"
              name="static_subnet"
              type="text"
              use:IPAddressMask
              disabled={!useStatic}
              bind:value={staticMask}
              placeholder="Enter subnet mask as xxx.xxx.xxx.xxx"
            />
          </div>
        </div>
      </div>
    </form>

    <div class="field">
      <div class="field-label" />
      <div class="control">
        <button
          on:click={doSave}
          class="button is-info"
          class:is-loading={isSubmitting}>Save and Restart</button
        >
      </div>
    </div>

    <div bind:this={successModalElem} class="modal">
      <div class="modal-background" />
      <div class="modal-content">
        <article class="message is-info">
          <div class="message-body">
            <p>Settings have been saved!</p>
            <p>
              Please wait for the device to restart. Attempting to reload the
              page in {countdownTimer}s
            </p>
          </div>
        </article>
      </div>
    </div>
  {/if}
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

  form {
    margin: 0 auto;

    &:not(:last-child) {
      margin-bottom: 50px;
    }
  }

  ul li {
    cursor: pointer;

    transition: background-color 0.5s;
    padding: 5px;

    background-color: hsl(0, 0%, 98%);

    &:not(:last-child) {
      margin-bottom: 5px;
    }
    border-radius: 5px;

    &.is-active {
      background-color: #5caef1;
      font-weight: bold;
    }

    &:not(.is-active):hover {
      background-color: #a0c9eb;
    }

    subtitle {
      margin-left: 5px;
      font-size: 0.7em;
      color: hsl(0, 0%, 41%);
    }
  }
</style>
