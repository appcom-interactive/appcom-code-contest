<template>
  <div class="player-wrapper">
    <div
      class="player"
      :class="{ 'flip-player': flipPlayer, moving: !disableMoving && moving, falling, jumping }"
    ></div>
  </div>
</template>

<script>
import { mapGetters, mapMutations } from 'vuex';
import Vue from 'vue';
import VueAnime from 'vue-animejs';

Vue.use(VueAnime);

export default {
  props: {
    flipPlayer: {
      type: Boolean,
      default: false
    },
    moving: {
      type: Boolean,
      default: false
    },
    pickBlock: {
      type: String,
      default: null
    }
  },
  data: () => ({
    targets: null,
    grid: null,
    size: 62,
    resetFunction: false,
    previousPosition: { x: 0, y: 0 },
    falling: false,
    jumping: false,
    disableMoving: false
  }),
  computed: {
    ...mapGetters(['player', 'selectedWorld', 'finished', 'selectedWorld', 'reset'])
  },
  watch: {
    async pickBlock(value) {
      if (value && value === 'left') {
        await this.$anime({
          targets: this.targets,
          translateX: this.player.x * this.size - 10,
          translateY: this.player.y * this.size,
          translateZ: 0,
          scale: 1,
          easing: 'linear',
          duration: 400
        }).finished;
      } else if (value && value === 'right') {
        await this.$anime({
          targets: this.targets,
          translateX: this.player.x * this.size + 10,
          translateY: this.player.y * this.size,
          translateZ: 0,
          scale: 1,
          easing: 'linear',
          duration: 400
        }).finished;
      } else {
        await this.$anime({
          targets: this.targets,
          translateX: this.player.x * this.size,
          translateY: this.player.y * this.size,
          translateZ: 0,
          scale: 1,
          easing: 'linear',
          duration: 400
        }).finished;
      }
    },
    finished(value) {
      setTimeout(async () => {
        this.resetFunction = true;
        this.previousPosition = { x: 0, y: 0 };
        this.$emit('animation-finished');

        if (value) {
          await this.$anime({
            targets: this.targets,
            easing: 'cubicBezier(.5, .05, .1, .3)',
            rotate: 750,
            scale: 0,
            duration: 1000
          }).finished;

          this.setShowFinishedModal(true);

          this.resetFunction = false;
        } else {
          await this.$anime({
            targets: this.targets,
            easing: 'cubicBezier(.5, .05, .1, .3)',
            rotate: 0,
            translateX: this.player.x * this.size,
            translateY: this.player.y * this.size,
            scale: 1,
            duration: 1000
          }).finished;

          this.resetFunction = false;
        }
      }, 500);
    },
    selectedWorld: {
      handler: async function() {
        this.resetFunction = true;

        this.$emit('animation-finished');

        this.previousPosition = {
          x: this.player.x,
          y: this.player.y
        };

        await this.$anime({
          targets: this.targets,
          translateX: this.player.x * this.size,
          translateY: this.player.y * this.size,
          scale: 0,
          rotate: 750,
          duration: 0
        }).finished;

        await this.$anime({
          targets: this.targets,
          translateY: this.player.y * this.size,
          rotate: 0,
          scale: 1,
          easing: 'cubicBezier(.5, .05, .1, .3)',
          duration: 1000
        }).finished;

        this.resetFunction = false;
      }
    },
    player: {
      handler: async function(value) {
        if (!this.resetFunction) {
          if (this.previousPosition === null || value.y === this.previousPosition.y) {
            await this.$anime({
              targets: this.targets,
              translateX: value.x * this.size,
              translateY: value.y * this.size,
              translateZ: 0,
              scale: 1,
              easing: 'linear',
              duration: 400
            }).finished;
          } else if (value.y > this.previousPosition.y) {
            await this.$anime({
              targets: this.targets,
              translateX: this.player.x * this.size + this.size / 2 + (value.x < this.previousPosition.x ? -20 : -40),
              easing: 'linear',
              duration: 400
            }).finished;

            this.falling = true;

            await this.$anime({
              targets: this.targets,
              translateY: this.player.y * this.size,
              duration: 200,
              easing: 'linear'
            }).finished;

            this.falling = false;

            await this.$anime({
              targets: this.targets,
              translateX: this.player.x * this.size,
              easing: 'linear',
              duration: 200
            }).finished;
          } else if (value.y < this.previousPosition.y) {
            await this.$anime({
              targets: this.targets,

              translateY: value.y * this.size + (this.size - 10),
              translateX: this.player.x * this.size + this.size / 2 + (value.x < this.previousPosition.x ? 20 : -80),
              duration: 300
            }).finished;

            this.jumping = true;

            setTimeout(() => {
              this.jumping = false;
            }, 300);

            await this.$anime({
              targets: this.targets,
              translateY: [{ value: value.y * this.size }, { value: value.y * this.size }, {}],
              translateX: [
                {
                  value: this.player.x * this.size - this.size / 2 + (value.x < this.previousPosition.x ? 60 : 0)
                },
                {},
                { value: value.x * this.size }
              ],
              easing: 'linear'
            }).finished;

            this.disableMoving = false;
          }

          this.previousPosition.x = value.x;
          this.previousPosition.y = value.y;
        }

        this.$emit('animation-finished');
      },
      deep: true
    }
  },
  async mounted() {
    this.targets = this.$el;
    const { world } = this.selectedWorld;

    const rows = world.length;
    const cols = world[0].length;

    this.grid = [cols, rows];
    this.size = 62;

    const startX = this.player.x * this.size;
    const startY = this.player.y * this.size;

    this.previousPosition.x = this.player.x;
    this.previousPosition.y = this.player.y;

    await this.$anime({
      targets: this.targets,
      translateX: startX,
      translateY: -100,
      rotate: 0,
      scale: 0.2
    }).finished;

    await this.$anime({
      targets: this.targets,
      translateY: startY,
      scale: 1,
      rotate: 0,
      easing: 'spring(1, 100, 100, 5)'
    }).finished;
  },
  methods: {
    ...mapMutations(['setPlayer', 'setShowFinishedModal'])
  }
};
</script>

<style lang="scss" scoped>
div.player-wrapper {
  position: absolute;
  top: 4px;
  left: 0;
  z-index: 10;

  div.player {
    height: 62px;
    width: 62px;

    background-image: url('../../assets/player_idle.gif');
    background-size: contain;
    background-repeat: no-repeat;
    background-position: center;

    &.moving {
      background-image: url('../../assets/run.gif');
    }

    &.flip-player {
      transform: scaleX(-1);
    }

    &.falling {
      background-image: url('../../assets/midair.gif');
    }

    &.jumping {
      background-image: url('../../assets/grab.gif');
    }
  }
}
</style>
