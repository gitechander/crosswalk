// Copyright 2014 The Chromium Authors. All rights reserved. 
// Use of this source code is governed by a BSD-style license that can be 
// found in the LICENSE file. 

console.error("loading file_picker.js");

cr.define('filePicker', function() {
  'use strict';

  function initialize() {
    console.error("Initialize...");
    //var args = JSON.parse(chrome.getVariableValue('dialogArguments'));
    
    $('save-button').addEventListener('click', function() {
    	console.error("save");
    	chrome.send('save');
      
    });
    $('cancel-button').addEventListener('click', function() {
    	console.error("cancel");
    	chrome.send('cancel');
      
    });

    $('save-button').innerText =
          loadTimeData.getStringF('saveButtonText');
    $('cancel-button').innerText =
          loadTimeData.getStringF('cancelButtonText');
    $('button-row').style['text-align'] = 'end';
  }

  return {
    initialize: initialize
  };
});

document.addEventListener('DOMContentLoaded',
                          filePicker.initialize);

