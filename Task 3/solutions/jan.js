module.exports = {
  split(value, separator) {
    const returnValue = [];
    if (!value) {
      return returnValue;
    }
    if (typeof value === 'string' && typeof separator !== 'string') {
      return [value];
    }
    if (typeof value === 'string') {
      let startPos = 0;
      let endPos;
      do {
        endPos = value.indexOf(separator, startPos);
        returnValue.push(value.substring(startPos, endPos >= 0 ? endPos : value.length));
        startPos = endPos + separator.length;
      } while (endPos !== -1);
    }
    if (typeof value === 'object') {
      for (const key in value) {
        returnValue.push({ [key]: value[key] });
      }
    }
    return returnValue;
  }
};