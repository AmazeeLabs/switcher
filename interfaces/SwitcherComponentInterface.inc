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

  /**
   * Returns a summary of a component based on an array of values.
   *
   * @param array $values
   *  An associative array where the keys represent the ids of the components
   *  and the values are the values of the components.
   */
  public function summary($values);

  /**
   * Prepares a switcher item that will be displayed.
   *
   * @param array $values
   *  The same as SwitcherComponentInterface::summary() $values.
   *
   * @param string $url
   *  The url which will be used to print in the switcher.
   *
   * @param array $options
   *  An array with the options of the URL.
   */
  public function prepareSwitcherItem(&$values, &$url, &$options);

  /**
   * Returns the current value of the component (from the current request).
   */
  public function getCurrentValue();
}