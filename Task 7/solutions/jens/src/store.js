import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    debug: {
      collisionTracking: false,
      showTileIDs: false,
      showCoordinates: false
    },
    worldId: 'PickBlocks',
    worlds: [],
    shiftLeft: 0,
    finished: false,
    showFinishedModal: false,
    reset: false,
    player: {
      x: undefined,
      y: undefined
    },
    message: null
  },
  mutations: {
    increaseShiftLeft(state) {
      state.shiftLeft += 1;
    },
    decreaseShiftLeft(state) {
      state.shiftLeft -= 1;
    },
    resetShiftLeft(state) {
      state.shiftLeft = 0;
    },
    setFinished(state, value) {
      state.finished = value;
    },
    setShowFinishedModal(state, value) {
      state.showFinishedModal = value;
    },
    setWorldId(state, worldId) {
      state.worldId = worldId;
    },
    setReset(state, value) {
      state.reset = value;
    },
    setPlayer(state, { x, y } = {}) {
      state.player.x = x;
      state.player.y = y;
    },
    setMessage(state, value) {
      state.message = value;
    }
  },
  actions: {},
  getters: {
    selectedWorld(state) {
      return state.worlds.find(world => world.id === state.worldId);
    },
    worlds(state) {
      return state.worlds;
    },
    worldId(state) {
      return state.worldId;
    },
    debug(state) {
      return state.debug;
    },
    shiftLeft(state) {
      return state.shiftLeft;
    },
    finished(state) {
      return state.finished;
    },
    showFinishedModal(state) {
      return state.showFinishedModal;
    },
    reset(state) {
      return state.reset;
    },
    player(state) {
      return state.player;
    },
    message(state) {
      return state.message;
    }
  }
});
