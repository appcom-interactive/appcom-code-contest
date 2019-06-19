<template>
  <div class="tile">
    <div v-if="condition" class="tile-type" :class="[`tile-${value}`, tileSpecificClasses]">
      <div v-if="debug.showTileIDs" class="text">{{value}}</div>
      <div v-if="debug.showCoordinates" class="text">{{coordinates}}</div>

      <transition name="highlight-transition">
        <div v-if="value === 'S' && showTutorial" class="tutorial">
          <p>Willkommen zu Block Mate!</p>
          <br>
          <p>Benutze die Pfeiltasten um dich in der Spielwelt zu bewegen.</p>
        </div>
      </transition>

      <transition name="highlight-transition">
        <div v-if="value === 'X' && showTutorial" class="tutorial">
          <p>Erreiche die Tür!</p>
        </div>
      </transition>

      <template v-if="debug.collisionTracking">
        <transition name="highlight-transition">
          <div v-if="shouldHighlight" class="highlight" :class="{ allowed: highlightAllowed}"></div>
        </transition>
      </template>
    </div>
  </div>
</template>

<script>
import { mapGetters } from 'vuex';
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
    }
  },
  computed: {
    typeSpecificClasses() {},
    ...mapGetters(['debug'])
  }
};
</script>

<style lang="scss" scoped>
div.tile {
  &.tile- {
    &1 {
      background: url('../assets/tileset.png') no-repeat;
      background-size: initial;
      background-position: -46px -30px;
      background-size: 1200px;

      &.underlying {
        background-position: -46px -78px;
      }
    }

    &S {
      background: url('../assets/player_idle.gif') no-repeat center;
      background-size: contain;

      &.moving {
        background: url('../assets/run.gif') no-repeat center;
        background-size: contain;
      }

      &.flip {
        transform: scaleX(-1);
      }

      &.starting {
        background: url('../assets/midair.gif') no-repeat center;
        background-size: contain;
      }
    }

    &X {
      z-index: 99;
      position: relative;

      &:before {
        content: '';
        position: absolute;
        background: rgb(107, 46, 18);
        width: 50%;
        height: 100%;
        left: 50%;
        transform: translateX(-50%);
        border: 1px solid rgba(0, 0, 0, 0.3);
        box-shadow: 0 0 0 rgb(177, 60, 5);
        animation: pulse 2s infinite;

        z-index: 99;
      }

      &:after {
        content: '';
        position: absolute;
        left: 50%;
        top: 0;
        width: 1px;
        height: 100%;
        background: rgba(0, 0, 0, 0.3);
        transform: translateX(-50%);
        z-index: 100;
      }
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
</style>
