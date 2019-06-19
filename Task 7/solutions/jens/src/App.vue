<template>
  <div class="wrapper">
    <Debug/>
    <World>
      <Parallax>
        <GameBoard/>
      </Parallax>
      <FinishedModal/>
    </World>
  </div>
</template>

<script>
import Debug from './components/Debug.vue';
import World from './components/World.vue';
import Parallax from './components/Parallax.vue';
import GameBoard from './components/GameBoard';

import FinishedModal from './components/FinishedModal.vue';

export default {
  components: {
    Debug,
    World,
    Parallax,
    GameBoard,
    FinishedModal
  }
};
</script>

<style lang="scss">
html {
  height: 100vh;
  width: 100vw;

  color: white;

  * {
    margin: 0;
    padding: 0;
  }
}

select {
  font-size: 18px;
}

div.wrapper {
  font-family: Helvetica, Courier, monospace;
  height: 100vh;
  width: 100vw;
  position: relative;
  background: black;

  > div.row {
    display: block;

    > div {
      display: inline-block;
    }
  }
}

@-webkit-keyframes pulse {
  0% {
    -webkit-box-shadow: 0 0 0 0 rgba(252, 156, 156, 0.479);
  }
  70% {
    -webkit-box-shadow: 0 0 0 10px rgba(204, 169, 44, 0);
  }
  100% {
    -webkit-box-shadow: 0 0 0 0 rgba(204, 169, 44, 0);
  }
}
@keyframes pulse {
  0% {
    -moz-box-shadow: 0 0 0 0 rgba(252, 156, 156, 0.479);
    box-shadow: 0 0 0 0 rgba(252, 156, 156, 0.479);
  }
  70% {
    -moz-box-shadow: 0 0 0 10px rgba(204, 169, 44, 0);
    box-shadow: 0 0 0 10px rgba(204, 169, 44, 0);
  }
  100% {
    -moz-box-shadow: 0 0 0 0 rgba(204, 169, 44, 0);
    box-shadow: 0 0 0 0 rgba(204, 169, 44, 0);
  }
}
</style>
