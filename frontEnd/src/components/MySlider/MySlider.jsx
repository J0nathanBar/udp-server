import React, { Component } from 'react'
import Slider from '@mui/joy/Slider';
export default class MySlider extends Component {
  render() {
    return (
      <div>
        <Slider
  color="primary"
  orientation="horizontal"
  valueLabelDisplay="on"
  variant="solid"
  max={3000}
  min={100}
  step={50}
  sx={{
    "width": "300px"
  }}
/>
        
      </div>
    )
  }
}
