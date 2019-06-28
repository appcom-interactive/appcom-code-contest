<template>
  <div class="debug-wrapper">
    <div class="settings row left">
      <select v-model="worldId">
        <option value="0" disabled>Bitte wählen Sie eine Welt aus</option>
        <option v-for="(world, index) in worlds" :key="index" :value="world.id">{{ world.title }}</option>
      </select>
    </div>
    <div class="settings row right">
      <label>
        Kollisionserkennungstracking anzeigen
        <input type="checkbox" v-model="collisionTracking">
      </label>
      <br>
      <label>
        Kachel IDs anzeigen
        <input type="checkbox" v-model="showTileIDs">
      </label>
      <br>
      <label>
        Koordinaten anzeigen
        <input type="checkbox" v-model="showCoordinates">
      </label>
    </div>
  </div>
</template>

<script>
import { mapGetters } from 'vuex';

export default {
  computed: {
    showTileIDs: {
      get() {
        return this.$store.state.debug.showTileIDs;
      },
      set(value) {
        this.$store.state.debug.showTileIDs = value;
      }
    },
    collisionTracking: {
      get() {
        return this.$store.state.debug.collisionTracking;
      },
      set(value) {
        this.$store.state.debug.collisionTracking = value;
      }
    },
    showCoordinates: {
      get() {
        return this.$store.state.debug.showCoordinates;
      },
      set(value) {
        this.$store.state.debug.showCoordinates = value;
      }
    },
    worldId: {
      get() {
        return this.$store.state.worldId;
      },
      set(value) {
        this.$store.state.worldId = value;
      }
    },
    ...mapGetters(['worlds'])
  }
};
</script>

<style lang="scss" scoped>
.debug-wrapper {
  .settings {
    position: absolute;
    top: 10px;
    opacity: 0.7;

    z-index: 99;

    &.left {
      left: 10px;
    }

    &.right {
      right: 10px;
      text-align: right;
    }
  }
}
</style>
