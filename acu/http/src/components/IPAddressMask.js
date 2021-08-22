import { imask } from '@imask/svelte'
import { MaskedNumber } from 'imask'

export default node =>
  imask(node, {
    mask: '#.#.#.#',
    blocks: {
      '#': {
        mask: MaskedNumber,
        scale: 0,
        min: 0,
        max: 255
      }
    }
  })
