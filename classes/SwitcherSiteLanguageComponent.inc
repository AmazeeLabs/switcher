<?php

/**
 * @file
 *  Switcher component for the language of the site.
 */

class SwitcherSiteLanguageComponent implements SwitcherComponentInterface {

  /**
   * (non-PHPdoc)
   * @see SwitcherComponentInterface::getTitle()
   */
  public function getTitle() {
    return t('Language');
  }

  /**
   * (non-PHPdoc)
   * @see SwitcherComponentInterface::getAvailableOptions()
   */
  public function getAvailableOptions() {
    $languages = language_list();
    foreach ($languages as $language_object) {
      $options[$language_object->language] = $language_object->name;
    }
    return $options;
  }
}
