<template>
  <transition name="finished-modal">
    <div v-if="showFinishedModal" class="finished-wrapper">
      <div class="finished">
        <p class="headline">Das ist das Ende!</p>
        <p>Wähle aus aus, ob du das Level neustarten oder in das nächste Level springen möchtest.</p>
        <div>
          <button @click="restart">Level neustarten</button>
          <div class="spacer">&nbsp;</div>
          <button @click="next">Nächstes Level</button>
        </div>
      </div>
    </div>
  </transition>
</template>

<script>
import { mapState, mapGetters, mapMutations } from 'vuex';

export default {
  computed: {
    ...mapGetters(['selectedWorld', 'worlds', 'worldId']),
    ...mapState(['showFinishedModal'])
  },
  methods: {
    restart() {
      this.setReset(true);
      this.setFinished(false);
      this.setShowFinishedModal(false);
    },
    next() {
      const index = this.worlds.findIndex(world => world.id === this.worldId);

      if (index < this.worlds.length - 1) {
        this.setWorldId(this.worlds[index + 1].id);
      } else {
        this.setWorldId(this.worlds[0].id);
      }

      this.setFinished(false);
      this.setShowFinishedModal(false);
      this.setReset(true);
    },
    ...mapMutations(['setWorldId', 'setFinished', 'setShowFinishedModal', 'setReset'])
  }
};
</script>

<style lang="scss" scoped>
.finished-wrapper {
  position: absolute;
  z-index: 1000;
  display: flex;
  align-items: center;
  justify-content: center;
  top: 0;
  right: 0;
  bottom: 0;
  left: 0;

  .finished {
    background: rgb(184, 220, 204);

    padding: 10px;
    text-align: center;
    color: black;
    box-shadow: 5px 5px 10px 10px rgba(0, 0, 0, 0.5);

    p {
      &.headline {
        font-size: 25px;
      }

      padding: 10px 0;

      &:first-child {
        white-space: nowrap;
      }
    }

    > div {
      padding-top: 10px;
      display: flex;

      button {
        flex: 1;

        -webkit-appearance: none;
        outline: 0;

        font-size: 16px;
        padding: 10px 0;
        background: transparent;
        border: 1px solid black;
        cursor: pointer;

        &:hover {
          background: rgb(139, 185, 165);
        }
      }
    }
  }
}

.finished-modal-enter-active,
.finished-modal-leave-active {
  transition: all 1s ease-in-out;
}
.finished-modal-enter {
  transition-delay: 2s;
  transform: translateY(-100%);
}

.finished-modal-leave-to {
  transform: translateY(-100%);
}
</style>
