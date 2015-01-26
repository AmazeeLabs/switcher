<?php

/**
 * @file
 *  Interface for all the Switcher components.
 */

interface SwitcherComponentInterface {

  /**
   * Returns the title of the component.
   */
  public function getTitle();

  /**
   * Returns an array with all the available options this component can have.
   */
  public function getAvailableOptions();
}