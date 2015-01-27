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

  /**
   * (non-PHPdoc)
   * @see SwitcherComponentInterface::summary()
   */
  public function summary($values) {
    $language_names = array();
    $languages = language_list();
    if (!empty($values['site_language'])) {
      $language_names[] = $languages[$values['site_language']]->name;
    }
    return t('Language: @languages', array('@languages' => implode(', ', $language_names)));
  }

  /**
   * (non-PHPdoc)
   * @see SwitcherComponentInterface::prepareSwitcherItem()
   */
  public function prepareSwitcherItem(&$values, &$url, &$options) {
    $languages = language_list();
    if (!empty($values['site_language'])) {
      $options['language'] = $languages[$values['site_language']];
    }
  }

  /**
   * (non-PHPdoc)
   * @see SwitcherComponentInterface::getCurrentValue()
   */
  public function getCurrentValue() {
    global $language;
    return $language->language;
  }
}
