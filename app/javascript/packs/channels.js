import { 
  loadActionCableSliders,
  loadActionCableSlidersMaster,
  rangeSlider,
  rangeSliderMaster,
  resetRangeSliderValue,
  resetRangeSliderValueMaster,
  toggleButton,
} from '../modules/mixer.js'

document.addEventListener('DOMContentLoaded', (event) => {
  loadActionCableSliders();
  loadActionCableSlidersMaster();
  initListeners();
});

function initListeners() {
  const recordButton = document.querySelector('[data-event-trigger-id="record"]');
  recordButton.addEventListener('click', function() {
    toggleButton(this);
  });

  const currentMasterVolume = document.querySelector('[data-event-trigger-id="currentMasterVolume"]');
  currentMasterVolume.addEventListener('click', function() {
    resetRangeSliderValueMaster(this);
  });

  const masterVolumeSlider = document.querySelector('[data-event-trigger-id="masterVolumeSlider"]');
  masterVolumeSlider.addEventListener('input', function() {
    rangeSliderMaster(this);
  });

  const soloButton = document.querySelector('[data-event-trigger-id="solo"]');
  soloButton.addEventListener('click', function() {
    toggleButton(this);
  });

  const muteButton = document.querySelector('[data-event-trigger-id="mute"]');
  muteButton.addEventListener('click', function() {
    toggleButton(this);
  });

  const currentVolume = document.querySelector('[data-event-trigger-id="currentVolume"]');
  currentVolume.addEventListener('click', function() {
    resetRangeSliderValue(this);
  });

  const volumeSlider = document.querySelector('[data-event-trigger-id="volumeSlider"]');
  volumeSlider.addEventListener('input', function() {
    rangeSlider(this);
  });
}
