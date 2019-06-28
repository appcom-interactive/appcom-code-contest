<template>
  <div class="moveable-tile" :style="calcStyle"></div>
</template>

<script>
export default {
  props: {
    position: {
      type: Object,
      default: () => ({})
    }
  },
  computed: {
    calcStyle() {
      return {
        top: `${(this.position.y - 1) * 62}px`,
        left: `${(this.position.x - 1) * 62}px`
      };
    }
  }
};
</script>

<style lang="scss" scoped>
div.moveable-tile {
  position: absolute;
  margin-top: 3px;
  height: 62px;
  width: 62px;
  background: url('../../assets/tileset.png') no-repeat;
  background-size: 1200px;
  background-position: -658px -376px;
  z-index: 1;
}
</style>
