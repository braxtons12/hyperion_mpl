# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

from sphinx.builders.html import StandaloneHTMLBuilder
import subprocess, os
subprocess.call("doxygen Doxyfile", shell=True)

project = "hyperion::mpl"
copyright = "2024, Braxton Salyer <braxtonsalyer@gmail.com>"
author = "Braxton Salyer <braxtonsalyer@gmail.com>"


# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration


extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "sphinx.ext.intersphinx",
    "sphinx.ext.autosectionlabel",
    "sphinx.ext.todo",
    "sphinx.ext.coverage",
    "sphinx.ext.mathjax",
    "sphinx.ext.ifconfig",
    "sphinx.ext.viewcode",
    "sphinx.ext.inheritance_diagram",
    "sphinx_sitemap",
    "breathe",
    "myst_parser",
]
myst_enable_extensions = [
    "amsmath",
    "colon_fence",
    "deflist",
    "dollarmath",
    "fieldlist",
    "html_admonition",
    "html_image",
    "linkify",
    "replacements",
    "smartquotes",
    "strikethrough",
    "substitution",
    "tasklist",
]

templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

highlight_language = "c++"

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

autosummary_generate = True
html_baseurl = "braxtons12.github.io/hyperion_mpl"
html_theme = "pydata_sphinx_theme"
html_theme_options = {
    "github_url": "https://github.com/braxtons12/hyperion_mpl",
    
    # Toc options
    "header_links_before_dropdown": 8,
    "show_toc_level": 1,
    "navigation_depth": 2,
    "show_nav_level": 0,
    "pygment_light_style": "manni",
    "pygment_dark_style": "one-dark",
    "navbar_start": ["navbar-logo", "theme-switcher"],
    "navbar_center": ["navbar-nav"],
    "navbar_end": ["navbar-icon-links"],
    "secondary_sidebar_items": ["page-toc"],
    "footer_items": ["copyright", "last-updated", "sphinx-version"],
}
html_sidebars = {
        "**": ["search-field", "navbar-nav", "sidebar-nav-bs", "sidebar-ethical-ads"]
}
html_context = {
    "default_mode": "dark"
}
html_static_path = ["_static"]
rst_prolog = """
.. role:: cpp(code)
    :language: cpp
    :class: highlight

.. role:: rust(code)
    :language: rust
    :class: highlight

.. role:: cmake(code)
    :language: cmake
    :class: highlight

.. role:: bash(code)
    :language: bash
    :class: highlight

.. role:: lua(code)
    :language: lua
    :class: highlight

.. role:: xmake(code)
    :language: lua
    :class: highlight
"""

breathe_projects = {
        "hyperion::mpl": "_build/xml"
}
breathe_default_project = "hyperion::mpl"
breathe_default_members = ('members')

