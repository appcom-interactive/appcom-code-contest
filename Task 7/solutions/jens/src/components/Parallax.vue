<template>
  <div class="parallax" :style="backgroundStyle">
    <slot></slot>
  </div>
</template>

<script>
import { mapGetters } from 'vuex';
export default {
  computed: {
    backgroundStyle() {
      return {
        backgroundPositionX: `${this.shiftLeft}px, ${this.shiftLeft * 2}px, ${this.shiftLeft * 3}px, ${this.shiftLeft *
          5}px`
      };
    },
    ...mapGetters(['shiftLeft'])
  }
};
</script>

<style lang="scss" scoped>
div.parallax {
  background-image: url('../assets/bg.png'), url('../assets/plx-4.png'), url('../assets/plx-2.png'),
    url('../assets/plx-3.png');
  transition: background-position 0.2s;
  background-size: auto 100%;
}
</style>
