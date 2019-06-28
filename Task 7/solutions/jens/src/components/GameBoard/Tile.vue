<template>
  <div class="tile">
    <div class="tile-type" :class="[`tile-${value}`, tileSpecificClasses, { hideTile }]">
      <div v-if="debug.showTileIDs" class="text">{{value}}</div>
      <div v-if="debug.showCoordinates" class="text">{{coordinates}}</div>
    </div>

    <transition name="highlight-transition">
      <div v-if="value === 'S' && showTutorial" class="tutorial">
        <p>Willkommen zu Block Mate!</p>
        <br>
        <p>Benutze die Pfeiltasten um dich in der Spielwelt zu bewegen.</p>
      </div>
    </transition>

    <transition name="highlight-transition">
      <div v-if="value === 'X' && showTutorial" class="tutorial">
        <p>Erreiche das Ziel!</p>
      </div>
    </transition>

    <transition name="highlight-transition">
      <div v-if="value === 'P' && showTutorial" class="tutorial">
        <p>Verwende die Leertaste</p>
      </div>
    </transition>

    <transition name="highlight-transition">
      <div v-if="showMessage && value === 'S' && !showTutorial" class="tutorial small">
        <p>{{showMessage}}</p>
      </div>
    </transition>

    <template v-if="debug.collisionTracking">
      <transition name="highlight-transition">
        <div v-if="shouldHighlight" class="highlight" :class="{ allowed: highlightAllowed}"></div>
      </transition>
    </template>
  </div>
</template>

<script>
import { mapGetters, mapMutations } from 'vuex';
export default {
  props: {
    value: {
      default: '0'
    },
    coordinates: {
      type: String
    },
    shouldHighlight: {
      type: Boolean,
      default: false
    },
    highlightAllowed: {
      type: Boolean,
      default: false
    },
    tileSpecificClasses: {
      type: Object,
      default: () => ({})
    },
    showTutorial: {
      type: Boolean,
      default: false
    },
    hideTile: {
      type: Boolean,
      default: false
    }
  },
  data: () => ({
    showMessage: null
  }),
  watch: {
    message(value) {
      if (value) {
        this.showMessage = value;

        setTimeout(() => {
          this.showMessage = null;
          this.setMessage(null);
        }, 1500);
      }
    }
  },
  computed: {
    ...mapGetters(['debug', 'message'])
  },
  methods: {
    ...mapMutations(['setMessage'])
  }
};
</script>

<style lang="scss" scoped>
div.tile {
  height: 100%;

  .tile-type {
    height: 100%;

    &.hideTile {
      visibility: hidden;
    }
  }

  .tile- {
    &1 {
      background: url('../../assets/tileset.png') no-repeat;
      background-position: -46px -30px;
      background-size: 1200px;

      &.underlying {
        background-position: -658px -376px;

        &.diagonalRight {
          background-position: -338px -493px;
        }

        &.diagonalLeft {
          background-position: -461px -493px;
        }

        &.airLeft {
          background-position: 0 -76px;
        }

        &.airRight {
          background-position: -93px -76px;

          &.diagonalLeft {
            position: relative;

            &:before {
              content: '';
              position: absolute;
              top: 0;
              left: 0;
              bottom: 0;
              width: 50%;

              background: url('../../assets/tileset.png') no-repeat;
              background-size: 1200px;
              background-position: -463px -491px;
            }
          }
        }

        &.airLeft.airRight {
          background-position: -93px -76px;
          position: relative;

          &:before {
            content: '';
            position: absolute;
            top: 0;
            left: 0;
            bottom: 0;
            width: 50%;

            background: url('../../assets/tileset.png') no-repeat;
            background-size: 1200px;
            background-position: -3px -76px;
          }
        }
      }

      &:not(.underlying) {
        &.airLeft {
          background-position: 0 -30px;
        }

        &.airRight {
          background-position: -93px -30px;
        }
      }
    }

    &S {
      visibility: hidden;
      background: url('../../assets/player_idle.gif') no-repeat center;
      background-size: contain;

      &.moving {
        background: url('../../assets/run.gif') no-repeat center;
        background-size: contain;
      }

      &.flip {
        transform: scaleX(-1);
      }
    }

    &P {
      background: url('../../assets/tileset.png') no-repeat;
      background-size: 1200px;
      background-position: -658px -376px;
      margin-top: 3px;
      z-index: inherit;
    }
  }

  position: relative;

  .highlight {
    &:after {
      content: '';
      position: absolute;
      top: 0;
      right: 0;
      bottom: 0;
      left: 0;
      background-color: rgba(255, 0, 0, 0.15);
    }

    &.allowed:after {
      background-color: rgba(0, 255, 21, 0.15);
    }
  }

  .text,
  .coordinates {
    position: absolute;
    color: rgba(255, 255, 255, 0.6);
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
  }

  .text {
    font-size: 24px;
  }

  .coordinates {
    font-size: 12px;
  }

  .tutorial {
    position: absolute;
    bottom: calc(100% + 15px);
    left: 50%;
    transform: translateX(-50%);
    background: white;
    padding: 10px;
    border-radius: 3px;
    background-color: rgb(184, 220, 204);
    box-shadow: 2px 2px 5px 5px rgba(0, 0, 0, 0.3);
    color: black;
    z-index: 99;

    &.small {
      font-size: 12px;
      max-width: 300px;
    }

    p:first-child {
      white-space: nowrap;
    }

    &:after {
      position: absolute;
      content: '';

      width: 0;
      height: 0;
      left: 50%;
      bottom: -10px;
      transform: translateX(-50%);
      border-left: 10px solid transparent;
      border-right: 10px solid transparent;

      border-top: 10px solid rgb(184, 220, 204);
    }
  }
}

.highlight-transition-enter-active {
  transition: opacity 0.2s;
}
.highlight-transition-leave-active {
  transition: opacity 0.5s;
}
.highlight-transition-enter, .highlight-transition-leave-to /* .fade-leave-active below version 2.1.8 */ {
  opacity: 0;
}
</style>
