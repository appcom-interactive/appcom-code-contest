<template>
  <div class="goal" :style="calcStyle">
    <div class="goal-inner"></div>
  </div>
</template>

<script>
import { mapGetters } from 'vuex';
import VueAnime from 'vue-animejs';
import Vue from 'vue';

Vue.use(VueAnime);

export default {
  props: {
    position: {
      type: Object,
      default: () => ({})
    }
  },
  watch: {
    async finished(value) {
      const target = this.$el;
      if (value) {
        await this.$anime({
          targets: target,
          scale: 2.5,
          duration: 1100
        }).finished;

        await this.$anime({
          targets: target,
          scale: 0,
          duration: 1000
        }).finished;
      } else {
        this.$anime({
          targets: target,
          scale: 1,
          duration: 500
        });
      }
    }
  },
  computed: {
    calcStyle() {
      return {
        top: `${this.position.y * 62}px`,
        left: `${this.position.x * 62}px`
      };
    },
    ...mapGetters(['finished'])
  },
  mounted() {}
};
</script>

<style lang="scss">
div.goal {
  position: absolute;
  height: 62px;
  width: 62px;
  z-index: 9;

  .goal-inner {
    position: relative;
    height: 100%;
    width: 100%;

    &:before {
      content: '';
      position: absolute;
      background: rgba(0, 0, 0, 0.9);
      border-radius: 50%;
      width: 70%;
      height: 70%;
      top: 58%;
      left: 50%;
      transform: translate(-50%, -50%);
      border: 1px solid rgba(0, 0, 0, 0.3);
      box-shadow: 0 0 0 rgba(0, 0, 0, 0.3);
      animation: pulse 2s infinite;

      z-index: 99;
    }
  }
}
</style>
