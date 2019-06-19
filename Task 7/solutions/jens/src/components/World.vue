<template>
  <div class="world-wrapper">
    <slot></slot>
  </div>
</template>

<script>
export default {};
</script>

<style lang="scss" scoped>
div.world-wrapper {
  overflow: hidden;

  background-image: radial-gradient(circle 60px at 38% 50%, rgb(228, 195, 7), rgb(255, 242, 172), rgb(184, 220, 204));

  position: absolute;
  top: 50%;
  transform: translateY(-50%);
  width: 100%;
  margin: auto;
}
</style>
