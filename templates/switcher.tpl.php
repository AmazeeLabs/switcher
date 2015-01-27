<?php

/**
 * @file
 *  Template file for the switcher.
 */

print '<div class="switcher-wrapper">';
  print '<div class="switcher-active">' . $active_item . '</div>';
  print $item_list;
print '</div>';